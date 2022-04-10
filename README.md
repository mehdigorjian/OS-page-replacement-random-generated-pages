# OS-page-replacement-random-generated-pages
Page replacement algorithms using random generated pages

The program simulates a paging system. At the start of the program, user should choose a page replacement algorithm from FIFO, LRU and Second-Chance. Then it asks the user to select between four-page frames and eight-page frames. On each cycle, the program reads the number of the referenced page from a file that was created by randomly writing 100 integers between 0 and 15 to a file within the program. The output makes a system call to generate a time stamp for each page reference and displays the system time, page referenced and whether a fault occurred for each reference, and calculates the total number of page faults for each algorithm.

`COMPILATION: gcc page_replace_random_pages.cpp -o page_replace_random_pages -lstdc++` \
`         RUN: ./page_replace_random_pages`
