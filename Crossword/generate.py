import sys
import math
from operator import itemgetter

from crossword import *


class CrosswordCreator():

    def __init__(self, crossword):
        """
        Create new CSP crossword generate.
        """
        self.crossword = crossword
        self.domains = {
            var: self.crossword.words.copy()
            for var in self.crossword.variables
        }

    def letter_grid(self, assignment):
        """
        Return 2D array representing a given assignment.
        """
        letters = [
            [None for _ in range(self.crossword.width)]
            for _ in range(self.crossword.height)
        ]
        for variable, word in assignment.items():
            direction = variable.direction
            for k in range(len(word)):
                i = variable.i + (k if direction == Variable.DOWN else 0)
                j = variable.j + (k if direction == Variable.ACROSS else 0)
                letters[i][j] = word[k]
        return letters

    def print(self, assignment):
        """
        Print crossword assignment to the terminal.
        """
        letters = self.letter_grid(assignment)
        for i in range(self.crossword.height):
            for j in range(self.crossword.width):
                if self.crossword.structure[i][j]:
                    print(letters[i][j] or " ", end="")
                else:
                    print("█", end="")
            print()

    def save(self, assignment, filename):
        """
        Save crossword assignment to an image file.
        """
        from PIL import Image, ImageDraw, ImageFont
        cell_size = 100
        cell_border = 2
        interior_size = cell_size - 2 * cell_border
        letters = self.letter_grid(assignment)

        # Create a blank canvas
        img = Image.new(
            "RGBA",
            (self.crossword.width * cell_size,
             self.crossword.height * cell_size),
            "black"
        )
        font = ImageFont.truetype("assets/fonts/OpenSans-Regular.ttf", 80)
        draw = ImageDraw.Draw(img)

        for i in range(self.crossword.height):
            for j in range(self.crossword.width):

                rect = [
                    (j * cell_size + cell_border,
                     i * cell_size + cell_border),
                    ((j + 1) * cell_size - cell_border,
                     (i + 1) * cell_size - cell_border)
                ]
                if self.crossword.structure[i][j]:
                    draw.rectangle(rect, fill="white")
                    if letters[i][j]:
                        w, h = draw.textsize(letters[i][j], font=font)
                        draw.text(
                            (rect[0][0] + ((interior_size - w) / 2),
                             rect[0][1] + ((interior_size - h) / 2) - 10),
                            letters[i][j], fill="black", font=font
                        )

        img.save(filename)

    def solve(self):
        """
        Enforce node and arc consistency, and then solve the CSP.
        """
        self.enforce_node_consistency()
        self.ac3()
        return self.backtrack(dict())

    def enforce_node_consistency(self):
        """
        Update `self.domains` such that each variable is node-consistent.
        (Remove any values that are inconsistent with a variable's unary
         constraints; in this case, the length of the word.)
        """
        # iterate over all variables and their domains and delete all words with lenght != lenght of the field
        for variable in self.crossword.variables:
            for word in self.domains[variable].copy():
                if len(word) != variable.length:
                    self.domains[variable].remove(word)

    def revise(self, x, y):
        """
        Make variable `x` arc consistent with variable `y`.
        To do so, remove values from `self.domains[x]` for which there is no
        possible corresponding value for `y` in `self.domains[y]`.

        Return True if a revision was made to the domain of `x`; return
        False if no revision was made.
        """
        revision = False

        #check if two variables overlaps and get coordinates of the overlap
        if self.crossword.overlaps[x,y]:
            k, j = self.crossword.overlaps[x,y]
            """iterate over domain of x variable and check that for every value there is
            a possible value in the domain of y variable. If there is not, remove this value
            from the domain of x and set revision to True"""
            # don't check for the same words!!!

            consistent = False
            #original domains

            for value_x in self.domains[x].copy():
                for value_y in self.domains[y]:

                    if value_x[k] == value_y[j]:
                        consistent = True

                # if there is at least one option continue the cycle
                if consistent:
                    consistent = False
                    continue
                # if there are no options, remove value from X's domain and set revision to True
                else:
                    revision = True
                    consistent = False
                    self.domains[x].remove(value_x)



            return revision

    def ac3(self, arcs=None):
        """
        Update `self.domains` such that each variable is arc consistent.
        If `arcs` is None, begin with initial list of all arcs in the problem.
        Otherwise, use `arcs` as the initial list of arcs to make consistent.

        Return True if arc consistency is enforced and no domains are empty;
        return False if one or more domains end up empty.
        """
        # create a list of all arcs (initial list)
        if not arcs:
            arcs = []
            for variable_x in self.crossword.variables:
                neighbors = self.crossword.neighbors(variable_x)
                while len(neighbors) != 0:
                    neighbor = neighbors.pop()
                    arc = {variable_x, neighbor}
                    if arc not in arcs:
                        arcs.append(arc)


        # iterate over all arcs and enforce arc consistency
        while len(arcs) != 0:
            x, y = arcs.pop(0)

            # if arc was revised I need to add new arcs
            if self.revise(x, y):
                #check if X's domain is empty to return FALSE
                if len(self.domains[x]) == 0:
                    return False
                #add new ars (Z, X) where Z is neighbor of X and not Y
                neighbors = self.crossword.neighbors(x)
                while len(neighbors) != 0:
                    neighbor = neighbors.pop()
                    if neighbor == y:
                        continue
                    arc = {neighbor, x}
                    if arc not in arcs:
                        arcs.append(arc)

        return True


    def assignment_complete(self, assignment):
        """
        Return True if `assignment` is complete (i.e., assigns a value to each
        crossword variable); return False otherwise.
        """
        if not assignment:
            return False

        if len(assignment) != len(self.crossword.variables):
            return False

        for key in assignment:
            if assignment[key] == None:
                return False
        return True

    def consistent(self, assignment):
        """
        Return True if `assignment` is consistent (i.e., words fit in crossword
        puzzle without conflicting characters); return False otherwise.
        """
        # check that every variable has a distinct word
        values = set(assignment.values())
        l = 0
        # count empty variables with None
        for key in assignment:
            if assignment[key] == None:
                l += 1
        if l == 0:
            l = 1
        if (len(values)+l-1) != len(assignment):
            return False

        # check that unary constraint is enforced (lenght of the word is equal to variable's lenght)
        for variable in assignment:
            if variable.length != len(assignment[variable]):
                return False

        # check that arc constraint is enforced
        for variable in assignment:
            #find neighbors variables
            neighbors = self.crossword.neighbors(variable)
                #check that there are the same letters at intersections
            for neighbor in neighbors:
                k, j = self.crossword.overlaps[variable, neighbor]
                if neighbor in assignment.keys():
                    if assignment[variable][k] != assignment[neighbor][j]:
                        return False
        return True

    def order_domain_values(self, var, assignment):
        """
        Return a list of values in the domain of `var`, in order by
        the number of values they rule out for neighboring variables.
        The first value in the list, for example, should be the one
        that rules out the fewest values among the neighbors of `var`.
        """

        # create a dict to store data about ruled out values for each choice in VAR's domain
        ruled_out = dict(zip(self.domains[var], [0]*len(self.domains[var])))

        # loop through all options in VAR's domain and through all neighbours of VAR and calculate how much values will be ruled out for each option in VAR's domain
        neighbors = self.crossword.neighbors(var)

        for value in self.domains[var]:
            for neighbor in neighbors:
                # ignore neighbors that are already in the assignment
                if neighbor in assignment:
                    continue

                k, j = self.crossword.overlaps[var, neighbor]
                # loop through the domain of neighbor see how many items are iliminated with this word in VAR
                for word in self.domains[neighbor]:
                    if value[k] != word[j]:
                        ruled_out[value] += 1


        sorted_ruled_out = dict(sorted(list(ruled_out.items()), key = itemgetter(1)))

        return list(sorted_ruled_out.keys())

    def select_unassigned_variable(self, assignment):
        """
        Return an unassigned variable not already part of `assignment`.
        Choose the variable with the minimum number of remaining values
        in its domain. If there is a tie, choose the variable with the highest
        degree. If there is a tie, any of the tied variables are acceptable
        return values.
        """
        variables = {}

        for variable in self.crossword.variables:
            if variable not in assignment.keys():
                variables[variable] = [len(self.domains[variable]), len(self.crossword.neighbors(variable))]

        sorted_variables = dict(sorted(list(variables.items()), key = lambda value : value[1][1], reverse = True))
        new_sorted_variables = dict(sorted(list(sorted_variables.items()), key = lambda value : value[1][0], reverse = False))

        return(list(new_sorted_variables.keys())[0])


    def backtrack(self, assignment):
        """
        Using Backtracking Search, take as input a partial assignment for the
        crossword and return a complete assignment if possible to do so.

        `assignment` is a mapping from variables (keys) to words (values).

        If no assignment is possible, return None.
        """
        if self.assignment_complete(assignment):
            return(assignment)

        #try new variable

        var = self.select_unassigned_variable(assignment)

        #order values in domain of var using least constrain heuristic
        ordered_domain = self.order_domain_values(var, assignment)

        for value in ordered_domain:
            new_assignment = assignment.copy()
            new_assignment[var] = value

            if self.consistent(new_assignment):
                result = self.backtrack(new_assignment)
                if result is not None:
                    return result
        return None

def main():

    # Check usage
    if len(sys.argv) not in [3, 4]:
        sys.exit("Usage: python generate.py structure words [output]")

    # Parse command-line arguments
    structure = sys.argv[1]
    words = sys.argv[2]
    output = sys.argv[3] if len(sys.argv) == 4 else None

    # Generate crossword
    crossword = Crossword(structure, words)
    creator = CrosswordCreator(crossword)
    assignment = creator.solve()

    # Print result
    if assignment is None:
        print("No solution.")
    else:
        creator.print(assignment)
        if output:
            creator.save(assignment, output)


if __name__ == "__main__":
    main()
