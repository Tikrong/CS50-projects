import os
import random
import re
import sys


DAMPING = 0.85
SAMPLES = 10000


def main():
    if len(sys.argv) != 2:
        sys.exit("Usage: python pagerank.py corpus")
    corpus = crawl(sys.argv[1])
    ranks = sample_pagerank(corpus, DAMPING, SAMPLES)
    print(f"PageRank Results from Sampling (n = {SAMPLES})")
    for page in sorted(ranks):
        print(f"  {page}: {ranks[page]:.4f}")
    ranks = iterate_pagerank(corpus, DAMPING)
    print(f"PageRank Results from Iteration")
    for page in sorted(ranks):
        print(f"  {page}: {ranks[page]:.4f}")


def crawl(directory):
    """
    Parse a directory of HTML pages and check for links to other pages.
    Return a dictionary where each key is a page, and values are
    a list of all other pages in the corpus that are linked to by the page.
    """
    pages = dict()

    # Extract all links from HTML files
    for filename in os.listdir(directory):
        if not filename.endswith(".html"):
            continue
        with open(os.path.join(directory, filename)) as f:
            contents = f.read()
            links = re.findall(r"<a\s+(?:[^>]*?)href=\"([^\"]*)\"", contents)
            pages[filename] = set(links) - {filename}

    # Only include links to other pages in the corpus
    for filename in pages:
        pages[filename] = set(
            link for link in pages[filename]
            if link in pages
        )

    return pages


def transition_model(corpus, page, damping_factor):
    """
    Return a probability distribution over which page to visit next,
    given a current page.

    With probability `damping_factor`, choose a link at random
    linked to by `page`. With probability `1 - damping_factor`, choose
    a link at random chosen from all pages in the corpus.
    """
     # calculate the amount of links on the page and amount of pages
    num_of_links = len(corpus[page])
    num_of_pages = len(corpus)

    #empty dictionary for storing probability distribution
    next_page_pd = {}

    # if there is no links on the page
    if num_of_links == 0:

        #fill the dictionary with all pages as keys and values 1/number of pages
        for key in corpus:
            next_page_pd[key] = 1/num_of_pages

        return next_page_pd

    # if there are links on the page
    else:

        #fill the dictionary with all pages as keys and values
        for key in corpus:
            if key in corpus[page]:
                next_page_pd[key] = (1-damping_factor)/num_of_pages + damping_factor/num_of_links
            else:
                next_page_pd[key] = (1-damping_factor)/num_of_pages

    return next_page_pd


def sample_pagerank(corpus, damping_factor, n):
    """
    Return PageRank values for each page by sampling `n` pages
    according to transition model, starting with a page at random.

    Return a dictionary where keys are page names, and values are
    their estimated PageRank value (a value between 0 and 1). All
    PageRank values should sum to 1.
    """

    # create a dict with all pages for storing number of appearences in sets
    samples_track = {}
    for key in corpus:
        samples_track[key] = 0

    # pick a random page from the set
    sample = random.choice(list(corpus))

    for i in range(n):
        #calculate probability distribution for the first page
        pd = transition_model(corpus, sample, DAMPING)

        #choose the next page using probability distribution
        sample = random.choices(list(pd.keys()), list(pd.values()), k=1)[0]

        #keep track of pages appearing in samples
        samples_track[sample] += 1

    #normalize the data to get probability distribution (devide values by number of samples made)
    for key in samples_track.keys():
        samples_track[key] = samples_track[key]/n

    return samples_track


def iterate_pagerank(corpus, damping_factor):
    """
    Return PageRank values for each page by iteratively updating
    PageRank values until convergence.

    Return a dictionary where keys are page names, and values are
    their estimated PageRank value (a value between 0 and 1). All
    PageRank values should sum to 1.
    """

    num_of_pages = len(corpus)

    # create a dictionary with initial page rank for every page
    page_ranks = {}
    for key in corpus:
        page_ranks[key] = 1/num_of_pages

    new_page_ranks = {}

    # for storing temporary probability of visiting the inspected page
    tmp = 0
    # for checking whether to continue the cycle or not
    go = True

    # calculate new rank for each page
    while go == True:
        for page in corpus:
            for key in corpus:
                num_of_links_on_page = len(corpus[key]) # amount of links on the possible parent page
                if num_of_links_on_page == 0:
                    tmp += page_ranks[key] / num_of_pages
                elif page in corpus[key]:
                    tmp += page_ranks[key] / num_of_links_on_page

            new_page_ranks[page] = tmp * damping_factor
            new_page_ranks[page] += (1-damping_factor) / num_of_pages
            tmp = 0

        # check whether new pagerans differs from the old one more than on 0.001
        go = False
        for key in corpus.keys():
            if abs(new_page_ranks[key] - page_ranks[key]) > 0.001:
                go = True

        page_ranks = new_page_ranks.copy()

    return page_ranks

if __name__ == "__main__":
    main()
