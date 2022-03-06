import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    # get a list of dictionaries with symbols and shares which current user owns
    stocks = db.execute("SELECT symbol, SUM (shares) FROM :user GROUP BY symbol HAVING SUM (shares) > 0",
                        user = session.get("username"))

    # get current user's cash amount
    cash = db.execute("SELECT cash FROM users WHERE id = :userid", userid = session.get("user_id"))
    total = cash[0]["cash"]


    # add information about stock names and current price and total price of current user's holdings
    # and calculate total amount of user's cash
    for stock in stocks:
        stock["name"] = lookup(stock["symbol"])["name"]
        stock["price"] = lookup(stock["symbol"])["price"]
        stock["total"] = stock["price"] * stock["SUM (shares)"]
        total += stock["total"]




    # calculate total amount of user's cash

    return render_template("index.html", stocks=stocks, cash = cash[0]["cash"], total = total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        # chicking that there is a symbol
        if not request.form.get("symbol"):
            return apology("Must provide stock symbol", 403)
        elif not request.form.get("shares"):
            return apology("Must provide a number of shares", 403)
        else:
            quote = lookup(request.form.get("symbol"))
            # checking that symbol is correct
            if quote == None:
                return apology("Invalid symbol", 403)
            # chicking that user has enough money
            row = db.execute("SELECT cash FROM users WHERE id = :userid", userid=session.get("user_id"))
            quote = lookup(request.form.get("symbol"))
            price = lookup(request.form.get("symbol"))["price"] * float(request.form.get("shares"))
            if row[0]["cash"] < price:
                return apology("Not enough money", 403)
            else:
                db.execute("INSERT into :username (symbol, shares, price) VALUES (:symbol, :shares, :price)", username = session["username"],
                            symbol=quote["symbol"], shares=request.form.get("shares"), price = quote["price"])
                db.execute("UPDATE users SET cash = cash - :price WHERE id = :userid", price = price, userid = session.get("user_id"))
                return apology("OK", 403)

    else:
        return render_template("buy.html", username=session.get("username"))


@app.route("/history")
@login_required
def history():
    operations = db.execute("SELECT * FROM :user", user = session.get("username"))

    return render_template("history.html", operations=operations)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["username"] = rows[0]["username"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        # ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 403)
        else:
            quote = lookup(request.form.get("symbol"))
            if quote == None:
                return apology("Invalid Symbol", 403)
            else:
                return render_template("quoted.html", name=quote["name"], price=usd(quote["price"]), symbol=quote["symbol"])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password was repeated correctly
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 403)

        else:
            # Query database for username
            rows = db.execute("SELECT * FROM users WHERE username = :username",
                              username=request.form.get("username"))

            # Ensure username does not exist already
            if len(rows) != 0:
                return apology("This username is already occupied", 403)

            # add information about new user into users table
            db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash_p)", username=request.form.get("username"),
                        hash_p = generate_password_hash(request.form.get("password")))

            # create new table for storing user's transactions with name = username
            db.execute("""CREATE TABLE :user ('number' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 'symbol' TEXT NOT NULL,
            'shares' INTEGER NOT NULL, 'price' NUMERIC NOT NULL, 'transacted_at' TEXT DEFAULT CURRENT_TIMESTAMP)""",
            user = request.form.get("username"))

            # Redirect user to home page
            return redirect("/login")

    # if user reached via GET method
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        # check that the entered number is correct
        if not request.form.get("shares"):
            return apology("Must provide positive number of shares")
        elif float(request.form.get("shares")) < 1:
            return apology("Must provide positive number of shares")
        # check that symbol was selected
        elif not request.form.get("symbol"):
            return apology("Must select a symbol")
        # check that users own the stock selected
        user_shares = db.execute("SELECT SUM (shares) FROM :user WHERE symbol = :symbol", user = session.get("username"),
                                    symbol = request.form.get("symbol"))
        if user_shares[0]['SUM (shares)'] < int(request.form.get("shares")):
            return apology("Not enough shares", 403)
        else:
            #calculate current price for pur shares
            quote = lookup(request.form.get("symbol"))
            cur_price = quote["price"] * int(request.form.get("shares"))
            # update cash amount of the user
            db.execute("UPDATE users SET cash = cash + :price WHERE id = :userid", price = cur_price, userid = session.get("user_id"))
            # add the transaction into database marking selling by negative amount of shares
            db.execute("INSERT into :username (symbol, shares, price) VALUES (:symbol, :shares, :price)", username = session["username"],
                            symbol=quote["symbol"], shares=int(request.form.get("shares"))*(-1), price = quote["price"])
            return apology("SOLD", 403);

    else:
        # getting a list of all the stocks the user has
        stocks = []
        tmp_stocks = db.execute("SELECT symbol FROM :user GROUP BY symbol HAVING SUM (shares) > 0", user = session.get("username"))
        for stock in tmp_stocks:
            stocks.append(stock["symbol"])

        return render_template("sell.html", stocks = stocks)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
