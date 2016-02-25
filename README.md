# Shadow Recruit top
Another project made by Jack Rosenthal (a.k.a. Shadow Recruit)

List of files that may explain what they do:

    src/info/...                What you expect
    src/main.cpp                Contains the entry point to the program
    src/utils/fmt.{cpp,h}       Formatting strings
    src/utils/display.{cpp,h}   Responsible for drawing the display
    src/utils/opts.{cpp,h}      Parses and stores options

Ideally, this `top` implementation should be run on a terminal of size `30x98`
or greater (any larger size will work just as fine).

I spent about 6 hours working on this project.

## Extra Features

What would be a project from Shadow Recruit without him creating extra work for
himself? So he did.

 * Running processes are highlighted in red
 * Prints as many processes as can fit on the display, however you can specify
   a maximum using the `-m` or `--max-proc` flag.
 * Status bar at bottom to show delay, sort key, user, hostname, date, and time

