# AAIA - TP PageRank

![PageRank border](img/presentation_image_cropped_with_text.jpg)

> This project is configured for Linux, NixOS

A la ﬁn des années 90, Brin et Page ont conçu l’algorithme PageRank utilisé par le moteur de recherche Google pour trier les réponses d’une requête par ordre d’importance décroissante. Le but du TP est de découvrir et implémenter cet algorithme.

## commands

#### Manual compilation

`gcc src/main.c -O3 -Wall -Werror -o exe`: compile executable.

#### Make compilation

`make dirs`: Before doing anything, create output directories for Make rules.

`make build`: Build by compiling project.

`make run`: Run compiled executable.

#### Manual run

`./bin/main`: direct run of the compiled executable.

## About premature optimization

Premature optimization is the root of all evil...

> “There is no doubt that the holy grail of efficiency leads to abuse. Programmers waste enormous amounts of time thinking about, or worrying about, the speed of noncritical parts of their programs, and these attempts at efficiency actually have a strong negative impact when debugging and maintenance are considered. **We *should* forget about small efficiencies, say about 97% of the time: premature optimization is the root of all evil.**
>
> Yet we should not pass up our opportunities in that critical 3%. A good programmer will not be lulled into complacency by such reasoning, he will be wise to look carefully at the critical code; but only after that code has been identified.”
>
> — From “Structured Programming with *go to* Statements” (Knuth, [1974](https://doi.org/10.1145/356635.356640)), with bold added for emphasis, as seen [here](https://effectiviology.com/premature-optimization/).

## Sorting algorithms

Sorting algorithms are techniques designed to rearrange a sequence of elements into a particular order, such as ascending or descending. They are fundamental in computer science for organizing and processing data efficiently. Many such algorithms exists, some being more efficient than others, and some being very simple to implement.

[play with sorting algorithms animation](https://www.toptal.com/developers/sorting-algorithms)

[compare sorting algorithm complexity](https://www.bigocheatsheet.com/)
