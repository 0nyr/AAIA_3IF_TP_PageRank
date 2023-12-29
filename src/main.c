#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// DiGraph representation using adjacency lists
typedef struct {
    int n; // number of vertices in the graph
    int* nbSucc; // for each 0<=i<n, nbSucc[i] = number of successors of i
    int** succ; // for each 0<=i<n and each 0<=j<nbSucc[i], succ[i][j]=jth successor of i
} DiGraph;

DiGraph* readDigraph(FILE *fp) {
    // return the DiGraph contained in file fp
    DiGraph *g = (DiGraph*)malloc(sizeof(DiGraph));
    if (fscanf(fp, "%d\n", &(g->n)) != 1 || g->n <= 0) {
        printf("erreur de lecture du fichier\n");
        exit(1);
    }
    g->nbSucc = (int*)malloc(g->n*sizeof(int));
    g->succ = (int**)malloc(g->n*sizeof(int*));
    int succ[g->n];
    for (int i=0; i<g->n; i++) {
        g->nbSucc[i] = 0;
        while (1) {
            if (fscanf(fp, "%d", &(succ[g->nbSucc[i]])) != 1 || succ[g->nbSucc[i]] >= g->n) {
                printf("erreur de lecture du fichier\n");
                exit(1);
            }
            if (succ[g->nbSucc[i]]<0) break;
            g->nbSucc[i]++;
        };
        g->succ[i] = (int*)malloc(g->nbSucc[i]*sizeof(int));
        memcpy(g->succ[i], succ, g->nbSucc[i]*sizeof(int));
    }
    return g;
}

void printDigraph(DiGraph *g) {
    printf("Number of vertices: %d\n", g->n);

    // For each vertex of g, display the list of its successors
    for (int i=0; i<g->n; i++) {
        printf("Vertex %d has %d successors: ", i, g->nbSucc[i]);
        for (int j=0; j<g->nbSucc[i]; j++)
            printf("%d ", g->succ[i][j]);
        printf("\n");
    }
}

typedef struct {
    int k; // step number of the PageRank algorithm
    double* scores; // for each 0<=i<n, scores[i] = score
} Scores;

void printScores(Scores* scores_at_k, int n) {
    printf("Scores at step %d: [", scores_at_k->k);
    for (int j=0; j<n; j++) {
        printf("%f", scores_at_k->scores[j]);
        if (j<n-1) printf(", ");
    }

    double sum = 0;
    for (int j=0; j<n; j++) {
        sum += scores_at_k->scores[j];
    }
    printf("] (sum: %f)\n", sum);
}

/* Print score like so:
Scores at step 0: [0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667]
Scores at step 1: [0.027778, 0.055556, 0.027778, 0.319444, 0.291667, 0.277778]
Scores at step 2: [0.009259, 0.023148, 0.013889, 0.423611, 0.224537, 0.305556]
*/
void printScoresArray(Scores** scores, int lastK, int n) {
    for (int i=0; i<=lastK; i++) {
        printScores(scores[i], n);
    }
}

Scores** pagerank(DiGraph* g, int k) {
    // return the scores of the PageRank algorithm up to step k
    Scores** scores = (Scores**)malloc((k+1)*sizeof(Scores*));

    // initial scores at step 0
    scores[0] = (Scores*)malloc(sizeof(Scores));
    scores[0]->k = 0;
    scores[0]->scores = (double*)malloc(g->n*sizeof(double));
    for (int j=0; j<g->n; j++) {
        // initial transition probabilities are uniform
        scores[0]->scores[j] = 1.0/(double)g->n;
    }
        
    // step k >= 1
    for (int i=1; i<=k; i++) {
        // initialization of the scores at step k
        scores[i] = (Scores*)malloc(sizeof(Scores));
        scores[i]->k = i;
        scores[i]->scores = (double*)malloc(g->n*sizeof(double));
        for (int j=0; j<g->n; j++)
            scores[i]->scores[j] = 0;
        
        // redistribution of the scores to all successors
        for(int j=0; j<g->n; j++) {
            for (int l=0; l<g->nbSucc[j]; l++) {
                scores[i]->scores[g->succ[j][l]] += scores[i-1]->scores[j]/(double)g->nbSucc[j];
            }
        }
    }
    return scores;
}

Scores** pagerankStochastic(DiGraph* g, int k) {
    // return the scores of the PageRank algorithm up to step k
    // Corrected to handle is_absorbing nodes
    Scores** scores = (Scores**)malloc((k+1)*sizeof(Scores*));

    // initial scores at step 0
    scores[0] = (Scores*)malloc(sizeof(Scores));
    scores[0]->k = 0;
    scores[0]->scores = (double*)malloc(g->n*sizeof(double));
    for (int j=0; j<g->n; j++) {
        // initial transition probabilities are uniform
        scores[0]->scores[j] = 1.0/(double)g->n;
    }

    // determine which nodes are absorbing
    int* is_absorbing = (int*)malloc(g->n*sizeof(int));
    for (int j=0; j<g->n; j++) {
        if (g->nbSucc[j] == 0) is_absorbing[j] = 1;
        else is_absorbing[j] = 0;
    }
        
    // step k >= 1
    for (int i=1; i<=k; i++) {
        // initialization of the scores at step k
        scores[i] = (Scores*)malloc(sizeof(Scores));
        scores[i]->k = i;
        scores[i]->scores = (double*)malloc(g->n*sizeof(double));
        
        // absorbing nodes: compute minimal score
        double minScore = 0;
        for (int j=0; j<g->n; j++) {
            if (is_absorbing[j] == 1) minScore += scores[i-1]->scores[j]/(double)g->n;
        }
        
        for (int j=0; j<g->n; j++)
            scores[i]->scores[j] = minScore;
        
        // redistribution of the scores to all successors
        for(int j=0; j<g->n; j++) {
            for (int l=0; l<g->nbSucc[j]; l++) {
                scores[i]->scores[g->succ[j][l]] += scores[i-1]->scores[j]/(double)g->nbSucc[j];
            }
        }
    }
    return scores;
}

Scores** pagerankErgodic(DiGraph* g, int k, double alpha) {
    // return the scores of the PageRank algorithm up to step k
    // Relies on the Random Surfer modification:
    // with probability alpha, the surfer follows a random link
    // hence all nodes are reachable from all nodes
    // with a minimum probability of (1-alpha)/n
    Scores** scores = (Scores**)malloc((k+1)*sizeof(Scores*));

    // initial scores at step 0
    scores[0] = (Scores*)malloc(sizeof(Scores));
    scores[0]->k = 0;
    scores[0]->scores = (double*)malloc(g->n*sizeof(double));
    for (int j=0; j<g->n; j++) {
        // initial transition probabilities are uniform
        scores[0]->scores[j] = 1.0/(double)g->n;
    }

    // determine which nodes are absorbing
    int* is_absorbing = (int*)malloc(g->n*sizeof(int));
    for (int j=0; j<g->n; j++) {
        if (g->nbSucc[j] == 0) is_absorbing[j] = 1;
        else is_absorbing[j] = 0;
    }

    // step k >= 1
    double surferScore = (1-alpha)/(double)g->n;
    for (int i=1; i<=k; i++) {
        // initialization of the scores at step k
        scores[i] = (Scores*)malloc(sizeof(Scores));
        scores[i]->k = i;
        scores[i]->scores = (double*)malloc(g->n*sizeof(double));

        // absorbing nodes: compute minimal score
        double minScore = 0;
        for (int j=0; j<g->n; j++) {
            if (is_absorbing[j] == 1) minScore += scores[i-1]->scores[j]/(double)g->n;
        }

        // minimum possible score now has a contribution from the surfer
        for (int j=0; j<g->n; j++)
            scores[i]->scores[j] = (alpha * minScore) + surferScore;

        // redistribution of the scores to all successors
        for(int j=0; j<g->n; j++) {
            for (int l=0; l<g->nbSucc[j]; l++) {
                // on successor nodes, add the score of the previous node
                scores[i]->scores[g->succ[j][l]] += alpha * (scores[i-1]->scores[j]/(double)g->nbSucc[j]);
            }
        }
    }

    return scores;
}

Scores* pagerankErgodicWithStopThreshold(
    DiGraph* g, 
    double alpha, 
    double threshold
) {
    // Return the last scores of the PageRank algorithm
    // when convergence is reached.
    // Relies on the Random Surfer modification:
    // Threshold is the minimum difference between two consecutive scores
    // such that |scores[i]->scores[j] - scores[i-1]->scores[j]| < threshold
    // Stops the algorithm when this condition is met
    
    // initial scores at step 0
    Scores* lastScores = (Scores*)malloc(sizeof(Scores));
    lastScores->k = 0;
    lastScores->scores = (double*)malloc(g->n*sizeof(double));
    for (int j=0; j<g->n; j++) {
        // initial transition probabilities are uniform
        lastScores->scores[j] = 1.0/(double)g->n;
    }

    // initialize current scores
    Scores* currentScores = (Scores*)malloc(sizeof(Scores));
    currentScores->scores = (double*)malloc(g->n*sizeof(double));

    // determine which nodes are absorbing
    int* is_absorbing = (int*)malloc(g->n*sizeof(int));
    for (int j=0; j<g->n; j++) {
        if (g->nbSucc[j] == 0) is_absorbing[j] = 1;
        else is_absorbing[j] = 0;
    }

    // step k >= 1
    double surferScore = (1-alpha)/(double)g->n;
    int i = 1;
    while (1) { // we will break when convergence is reached
        // initialization of the scores at step k
        currentScores->k = i;
        if (i%10 == 0)
            printf(" *");
        else
            printf("*");
        

        // absorbing nodes: compute minimal score
        double minScore = 0;
        for (int j=0; j<g->n; j++) {
            if (is_absorbing[j] == 1) minScore += lastScores->scores[j]/(double)g->n;
        }

        // minimum possible score now has a contribution from the surfer
        for (int j=0; j<g->n; j++)
            currentScores->scores[j] = (alpha * minScore) + surferScore;

        // redistribution of the scores to all successors
        for(int j=0; j<g->n; j++) {
            for (int l=0; l<g->nbSucc[j]; l++) {
                // on successor nodes, add the score of the previous node
                currentScores->scores[g->succ[j][l]] += alpha * (lastScores->scores[j]/(double)g->nbSucc[j]);
            }
        }

        // check if we can stop the algorithm
        int stop = 1;
        for (int j=0; j<g->n; j++) {
            // WARN: Don't confuse abs() and fabs() !
            if (fabs(currentScores->scores[j] - lastScores->scores[j]) > threshold) {
                stop = 0; // we can't stop yet
                break;
            }
        }

        //printScores(currentScores, g->n);

        if (stop == 1) {
            printf("\n -> Convergence reached: stopping at step %d\n", i);
            break;
        }

        // prepare next iteration
        i++;
        // copy current scores to last scores
        memcpy(lastScores->scores, currentScores->scores, g->n*sizeof(double));
        lastScores->k = currentScores->k;
    }

    return lastScores;
}

void printRanks(DiGraph* g, Scores* lastScores) {
    // Print the ranks of the nodes in the graph
    // Ranks are computed from the final scores (last step)
    
    printf("Last scores provided at step %d\n", lastScores->k);

    // sort scores, while keeping track of the original index (node number)
    int* sortedIndices = (int*)malloc(g->n*sizeof(int));
    for (int i=0; i<g->n; i++) {
        sortedIndices[i] = i;
    }

    // sort indices using bubble sort
    // not the most efficient, but it's ok for small arrays
    for (int i=0; i<g->n; i++) {
        for (int j=0; j<g->n-i-1; j++) {
            if (lastScores->scores[j] < lastScores->scores[j+1]) {
                // swap scores
                double tmpScore = lastScores->scores[j];
                lastScores->scores[j] = lastScores->scores[j+1];
                lastScores->scores[j+1] = tmpScore;

                // swap indices
                int tmpIndex = sortedIndices[j];
                sortedIndices[j] = sortedIndices[j+1];
                sortedIndices[j+1] = tmpIndex;
            }
        }
    }

    // print ranks
    printf("┌────── Ranks [k=%d] ──────────────────────\n", lastScores->k);
    for (int i=0; i<g->n; i++) {
        printf("├─ Rank %d: node %d (score: %f)\n", i+1, sortedIndices[i], lastScores->scores[i]);
    }
}

int main() {
    // # Exercice 1: Load a first example graph
    printf("\nExercice 1:\n");
    FILE* fp  = fopen("res/example_1.txt", "r");
    DiGraph* g = readDigraph(fp);
    fclose(fp);

    printDigraph(g);

    // Compute the PageRank scores of the first example graph
    int k = 4;
    Scores** scores = pagerank(g, k);
    printScoresArray(scores, k, g->n);

    // # Exercice 2: Load a second example graph
    printf("\nExercice 2:\n");
    fp  = fopen("res/example_2.txt", "r");
    g = readDigraph(fp);
    fclose(fp);

    printDigraph(g);

    // Compute the PageRank scores of the second example graph
    k = 4;
    scores = pagerank(g, k);
    printScoresArray(scores, k, g->n);

    // use corrected version to handle is_absorbing nodes
    printf("\nExercice 2 with corrected algo:\n");
    scores = pagerankStochastic(g, k);
    printScoresArray(scores, k, g->n);

    // # Exercice 6: Random Surfer modification
    printf("\nExercice 6 with random surfer:\n");
    double alpha = 0.9;
    scores = pagerankErgodic(g, k, alpha);
    printScoresArray(scores, k, g->n);

    // # Exercice 7: Random Surfer modification with stop threshold
    double threshold = 0.0000000001;
    printf("\nExercice 7 with random surfer and stop threshold:\n");
    Scores* lastScores = pagerankErgodicWithStopThreshold(g, alpha, threshold);
    printRanks(g, lastScores);

    // # Exercice 7 bis: Load a third example graph
    printf("\nExercice 7 bis, on big GENETIC graph:\n");
    fp  = fopen("res/genetic.txt", "r");
    g = readDigraph(fp);
    fclose(fp);

    printDigraph(g);

    // Compute the PageRank scores of the second example graph
    lastScores = pagerankErgodicWithStopThreshold(g, alpha, threshold);
    printScores(lastScores, g->n);
    printRanks(g, lastScores);

    return 0;
}
