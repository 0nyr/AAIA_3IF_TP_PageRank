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
