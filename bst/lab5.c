/* lab5.c 
 * Lab5: Binary Search Trees
 * ECE 2230, Spring 2024*
 * This file contains drivers to test the BST package.
 *
 * The unit driver builds a custom tree and removes selected keys 
 *   -u 0:  run the unit driver with a custom tree, example test with removes
 *   -u 1:  run the unit driver with a custom tree, (48) is missing its right-left child and (16) is missing its left-right child
 *   -u 2:  run the unit driver with a custom tree, example deletion with many children
 *   -u 3:  run the unit driver with a custom tree, check replace for duplicate key
 *   -u 4:  test condition of rotating root of AVL tree while maintaining BST rules (lesser keys on left, greater on right) 
 *          uses tree from ECE 2230 HW#6 Problems #7 - #8, as it tested rotation of root (two times)
 *   -u 5:  test worst case tree that is left heavy, AVL should balance while BST stays in worst case
 *   -u 6:  test extreme values of key to prove Tree can handle any int key value 
 *
 * There are three drivers to build a tree using bst_insert and then access
 * keys in the tree using bst_access.  Use
 *   -o run the driver with an optimal tree
 *   -r run the driver with a randomly generated tree
 *   -p run the driver with a poor order for inserting keys 
 *   -w to set the number of levels in the initial tree
 *   -t to set the number of access trials 
 *
 * Another test driver tests random inserts and deletes.  This driver builds
 * an initial tree that is random, and then performs insertions and deletions
 * with equal probability.
 *   -e run the equilibrium driver
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h> 
#include <limits.h> //Added for unit driver 6, to test extreme values

#include "bst.h"

/* constants used with Global variables */
#define TRUE 1
#define FALSE 0

/* Global variables for command line parameters */
int SearchPolicy = BS_TREE;
int Seed = 1272467;
int Verbose = FALSE;
int EquilibriumTest = FALSE;
int OptimalTest = FALSE;
int RandomTest = FALSE;
int PoorTest = FALSE;
int WorstTest = FALSE;
int UnitNumber = -1;
int Levels = 16;
int Trials = 100000;

enum testtypes {OPTIMAL, RANDOM, POOR};//, WORST};


/* prototypes for functions in this file only */
void getCommandLine(int argc, char **argv);
void equilibriumDriver(void);
void accessDriver(int);
void unitDriver(const int *, const int, const int *, const int);

void build_optimal(BST *T, int levels);
void build_random(BST *T, int nodes);
void build_poor(BST *T, int nodes);

int main(int argc, char **argv)
{
    getCommandLine(argc, argv);
    printf("Seed: %d\n", Seed);
    srand48(Seed);


    /* ----- build custom tree and remove nodes ----- */
    if (UnitNumber == 0)                        /* enabled with -u flag */
    {
	// example test to remove leaves, 12 and 20, then internal nodes
	// 8, 24, 40 with one child, then 16, 48 with two children
	const int ins[] = {32,16,8,24,4,12,28,48,40,56,44,42,46};
	const int del[] = {12,20,8,24,40,16,48}; 
        unitDriver(ins, sizeof ins / sizeof(int),
		   del, sizeof del / sizeof(int));
    }
    if (UnitNumber == 1)
    {
	// example tests: (48) is missing its right-left child and
	//                (16) is missing its left-right child
	const int ins1[] = {32,16,48,8,24,40,56,4,20,28,36,44,60};
	const int del1[] = {16,48,32}; 
        unitDriver(ins1, sizeof ins1 / sizeof(int),
		   del1, sizeof del1 / sizeof(int));

	// example tests: (16) is missing its right-left child and 
	//                (48) is missing its left-right child
	const int ins1b[] = {32,16,48,8,24,40,56,4,12,28,36,52,60};
	const int del1b[] = {16,48,32}; 
        unitDriver(ins1b, sizeof ins1b / sizeof(int),
		   del1b, sizeof del1b / sizeof(int));
    }
    if (UnitNumber == 2)
    {
	// example deletion with many children
	const int ins[] = {200,100,50,150,25,75,125,175,65,85,135,80,130,140,78,82};
	const int del[] = {100,85,125}; 
        unitDriver(ins, sizeof ins / sizeof(int),
		   del, sizeof del / sizeof(int));
    }
    if (UnitNumber == 3)
    {
	// check replace for duplicate key
	const int ins[] = {10, 10};
	const int del[] = {10}; 
        unitDriver(ins, sizeof ins / sizeof(int),
		   del, sizeof del / sizeof(int));
    }
    /* -----  test condition of rotating root of AVL tree while maintaining BST rules ----- */
    if (UnitNumber == 4)                        /* enabled with -u flag */
    {
        const int ins[] = {14,6,20,12,1,5,4,22,24,26,28,30};
        int *ip;
        int rotations = 0;
        BST *Tree = bst_construct(SearchPolicy); //Create new tree, intended for AVL tree
        //Insert nodes into the tree
        for (int i = 0; i < 12; i++) 
        {
            ip = (int *) malloc(sizeof(int)); 
            *ip = ins[i];
            bst_insert(Tree, ins[i],ip); 
            rotations += bst_rotations(Tree); //Store amount of rotations that occur
        } 
        
        bst_debug_print_tree(Tree); 
        printf("Internal Path Length of tree is %d\n", bst_int_path_len(Tree)); 
        printf("# of Rotations of tree is %d\n", rotations);
        bst_destruct(Tree);
    }    
    /* ----- test worst case tree that is left heavy----- */
    if (UnitNumber == 5)                        /* enabled with -u flag */
    {
        const int ins[] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
        int *ip;
        int rotations = 0;
        BST *Tree = bst_construct(SearchPolicy); //Create new tree, intended for AVL tree
        //Insert nodes into the tree
        for (int i = 0; i < 21; i++) 
        {
            ip = (int *) malloc(sizeof(int)); 
            *ip = ins[i];
            bst_insert(Tree, ins[i],ip); 
            rotations += bst_rotations(Tree); //Store amount of rotations that occur
        } 
        
        bst_debug_print_tree(Tree); 
        printf("Internal Path Length of tree is %d\n", bst_int_path_len(Tree)); 
        printf("# of Rotations of tree is %d\n", rotations);
        bst_destruct(Tree);
    } 
    /* ----- Test extreme values of key to prove Tree can handle any int key value ----- */
    if (UnitNumber == 6)                        /* enabled with -u flag */
    {
	    //Create an array the goes from values INT_MIN < Array > INT_MAX, as these are the values the tree can take
        const int ins[] = {INT_MIN + 1, 3*(INT_MIN/4), INT_MIN/2, INT_MIN/4, 0, INT_MAX/4, INT_MAX/2, 3*(INT_MAX/4),INT_MAX - 1};
	    int *ip;
        int rotations = 0;
        BST *Tree = bst_construct(SearchPolicy); //Create new tree, intended for AVL tree
        //Insert nodes into the tree
        for (int i = 0; i < 9; i++) 
        {
            ip = (int *) malloc(sizeof(int)); 
            *ip = ins[i];
            bst_insert(Tree, ins[i],ip); 
            rotations += bst_rotations(Tree); //Store amount of rotations that occur
        } 
        
        bst_debug_print_tree(Tree); 
        printf("Internal Path Length of tree is %d\n", bst_int_path_len(Tree)); 
        printf("# of Rotations of tree is %d\n", rotations);
        bst_destruct(Tree);
    }

    /* ----- large tree tests  ----- */
    if (OptimalTest)                       /* enable with -o flag */
        accessDriver(OPTIMAL);
    if (RandomTest)                        /* enable with -r flag */
        accessDriver(RANDOM);
    if (PoorTest)                          /* enable with -p flag */
        accessDriver(POOR);
    //if (WorstTest)                         /* enable with -q flag */
    //    accessDriver(WORST);

    /* test for performance in equilibrium */
    if (EquilibriumTest)                   /* enable with -e flag */
        equilibriumDriver();

    return 0;
}

/* driver to test deletion of keys from custom tree.  
 *
 * ins_keys - array with the list of keys to insert into the tree
 * num_inserts - number of keys to insert
 *
 * del_keys - array with the list of keys to remove from the tree
 * num_deletes - number of keys to delete
 */
void unitDriver(const int ins_keys[], const int num_inserts,
	        const int del_keys[], const int num_deletes)
{
    int i;
    int *ip;
    BST *del_tree;
    Data dp;

    del_tree = bst_construct(SearchPolicy);

    printf("\n\n ====== Unit Driver ======\n\n");
    printf("Inserting %d items into tree\n", num_inserts);
    for (i = 0; i < num_inserts; i++) {
        ip = (int *) malloc(sizeof(int));
        *ip = ins_keys[i]; 
        bst_insert(del_tree, ins_keys[i], ip);
    }
    
    printf("Created tree for testing removes\n");
    bst_debug_print_tree(del_tree);
    
    printf("Removing %d items from tree\n", num_deletes);
    for (i = 0; i < num_deletes; i++) {
        printf(" -- Test (%d) about to remove key %d\n", i, del_keys[i]);
        dp = bst_remove(del_tree, del_keys[i]);
        bst_debug_print_tree(del_tree);
	if (dp != NULL)
	    assert(*(int *)dp == del_keys[i]);
	else
	    printf("\n\nFAILED to find the key in the tree ######\n");
        free(dp);
    }
    /* remove and free all items from tree */
    bst_destruct(del_tree);
}


/* driver to build and test trees.  Creates tree with half of keys
 * in tree and half missing.  Uses access to find random keys.  Note that this
 * algorithm does not delete keys from the tree.
 *
 * test_type - type of test (i.e. OPTIMAL, RANDOM, POOR)
 */
void accessDriver(int test_type)
{
    int i;
    int range_num_ints;
    int size;
    int key;
    int ipl, epl;
    int suc_search, suc_trials, unsuc_search, unsuc_trials;
    BST *test_tree;
    Data dp;
    clock_t start, end;

    /* print parameters for this test run */
    printf("\n----- Access driver -----\n");
    printf("  Access trials: %d\n", Trials);
    printf("  Levels for tree: %d\n", Levels);

    range_num_ints = pow(2, Levels);

    /* build tree.  Key range is twice size of tree and each key 
     * is either even or odd.
     */
    test_tree = bst_construct(SearchPolicy);
    printf("  Build");
    if (test_type == OPTIMAL) {
        printf(" optimal");
        build_optimal(test_tree, Levels);
    } else if (test_type == RANDOM) {
        printf(" random");
        build_random(test_tree, pow(2,Levels)-1);
    } else if (test_type == POOR) {
        printf(" poor");
        build_poor(test_tree, Levels);
    }/* else if (test_type == WORST) {
        printf(" worst");
        range_num_ints = build_avl_fib(test_tree, Levels) + 1;
    } */else {
        printf("invalid option in access test?\n");
        exit(1);
    }
    printf(" tree with size=%d\n", range_num_ints-1);

    if (Verbose)
        bst_debug_print_tree(test_tree);
    size = bst_size(test_tree);
    assert(size == range_num_ints-1);

    if (Trials > 0) {
        /* access tree to measure successful and unsuccessful search calls */
        suc_search = suc_trials = unsuc_search = unsuc_trials = 0;
        start = clock();
        for (i = 0; i < Trials; i++) {
            /* random key with uniform distribution */
            key = ((int) (drand48() * (range_num_ints * 2 - 1))) + 1;
            dp = bst_access(test_tree, key);
            if (dp == NULL) {
                unsuc_search += bst_key_comps(test_tree);
                unsuc_trials++;
            } else {
                suc_search += bst_key_comps(test_tree);
                suc_trials++;
                assert(*(int *)dp == -key);
            }
        }
        end = clock();
        assert(size == bst_size(test_tree));
        ipl = bst_int_path_len(test_tree);
        epl = ipl + 2 * size;
        printf("  After access exercise, time=%g, tree size=%d\n",
                1000*((double)(end-start))/CLOCKS_PER_SEC, size);
        if (suc_trials > 0)
            printf("    Expect successful search=%g, measured=%g, trials=%d\n", 
                    (2.0*ipl+size)/size,
                    (double) suc_search/suc_trials, suc_trials);
        if (unsuc_trials > 0)
            printf("    Expect unsuccessful search=%g, measured=%g, trials=%d\n", 
                    2.0*epl/(size+1),
                    (double) unsuc_search/unsuc_trials, unsuc_trials);
    }

    bst_debug_validate(test_tree);
    /* remove and free all items from tree */
    bst_destruct(test_tree);
    printf("----- End of access driver -----\n\n");
}

/* driver to test sequence of inserts and deletes.
 */
void equilibriumDriver(void)
{
    int i;
    int range_num_ints;
    int size;
    int key;
    int ipl, epl;
    int suc_search, suc_trials, unsuc_search, unsuc_trials;
    int keys_added, keys_removed;
    int *ip;
    BST *test_tree;
    Data dp;
    clock_t start, end;

    /* print parameters for this test run */
    printf("\n----- Equilibrium test driver -----\n");
    printf("  Trials in equilibrium: %d\n", Trials);
    printf("  Levels in initial tree: %d\n", Levels);

    range_num_ints = pow(2, Levels);

    /* build a random tree as starting point */
    test_tree = bst_construct(SearchPolicy);
    build_random(test_tree, pow(2,Levels)-1);
    size = bst_size(test_tree);
    ipl = bst_int_path_len(test_tree);
    epl = ipl + 2 * size;
    printf("  Initial random tree size=%d\n", size);
    printf("  Expect successful search for initial tree=%g\n", 
            (2.0*ipl+size)/size);
    printf("  Expect unsuccessful search for initial tree=%g\n", 
            2.0*epl/(size+1));
    if (Verbose)
        bst_debug_print_tree(test_tree);

    /* in equilibrium make inserts and removes with equal probability */
    suc_search = suc_trials = unsuc_search = unsuc_trials = 0;
    keys_added = keys_removed = 0;
    start = clock();
    for (i = 0; i < Trials; i++) {
        key = ((int) (drand48() * range_num_ints * 2)) + 1;
        if (Verbose) printf("Trial %d, Key %d", i, key);
        if (drand48() < 0.5) {
            ip = (int *) malloc(sizeof(int));
            *ip = -key;
            /* insert returns 1 if key not found, 0 if found */
            if (bst_insert(test_tree, key, ip) == 1) {
                unsuc_search += bst_key_comps(test_tree);
                unsuc_trials++;
                keys_added++;
                if (Verbose) printf(" added\n");
            } else {
                suc_search += bst_key_comps(test_tree);
                suc_trials++;
                if (Verbose) printf(" replaced\n");
            }
	    //if (Verbose && SearchPolicy == AVL)
		//printf("  rotations %d\n", bst_rotations(test_tree));
        } else {
            dp = bst_remove(test_tree, key);
            if (dp == NULL) {
                if (Verbose) printf(" not found\n");
                unsuc_search += bst_key_comps(test_tree);
                unsuc_trials++;
            } else {
                if (Verbose) printf(" removed\n");
                suc_search += bst_key_comps(test_tree);
                suc_trials++;
                keys_removed++;
                assert(*(int *)dp == -key);
                free(dp);
            }
	    //if (Verbose && SearchPolicy == AVL)
		//printf("  rotations %d\n", bst_rotations(test_tree));
        }
        /* this prints the tree after each trial */
        if (Verbose)
            bst_debug_print_tree(test_tree);
    }
    end = clock();
    /* print the final tree after all trials */
    //if (Verbose)
    //    bst_debug_print_tree(test_tree);

    size += keys_added - keys_removed;
    //printf("added %d removed %d new size %d tree size %d\n", keys_added,
    //        keys_removed, size, bst_size(test_tree));
    assert(size == bst_size(test_tree));
    printf("  After exercise, time=%g, new tree size=%d\n",
            1000*((double)(end-start))/CLOCKS_PER_SEC, size);
    printf("  successful searches during exercise=%g, trials=%d\n", 
            (double) suc_search/suc_trials, suc_trials);
    printf("  unsuccessful searches during exercise=%g, trials=%d\n", 
            (double) unsuc_search/unsuc_trials, unsuc_trials);

    printf("  Validating tree...");
    bst_debug_validate(test_tree);
    printf("passed\n");

    /* test access times for new tree */
    suc_search = suc_trials = unsuc_search = unsuc_trials = 0;
    start = clock();
    for (i = 0; i < Trials; i++) {
        key = ((int) (drand48() * range_num_ints * 2)) + 1;
        dp = bst_access(test_tree, key);
        if (dp == NULL) {
            unsuc_search += bst_key_comps(test_tree);
            unsuc_trials++;
        } else {
            suc_search += bst_key_comps(test_tree);
            suc_trials++;
        }
    }
    end = clock();
    size = bst_size(test_tree);
    ipl = bst_int_path_len(test_tree);
    epl = ipl + 2 * size;
    printf("  After access experiment, time=%g, tree size=%d\n",
            1000*((double)(end-start))/CLOCKS_PER_SEC, size);
    printf("  Expect successful search=%g, measured=%g, trials=%d\n", 
            (2.0*ipl+size)/size,
            (double) suc_search/suc_trials, suc_trials);
    printf("  Expect unsuccessful search=%g, measured=%g, trials=%d\n", 
            2.0*epl/(size+1),
            (double) unsuc_search/unsuc_trials, unsuc_trials);

    /* remove and free all items from tree */
    bst_destruct(test_tree);

    printf("----- End of equilibrium test -----\n\n");
}

/* create one node to be inserted in tree with an even key
 *
 * T - tree to insert into
 * key - key for the new node
 */
void build_one_node(BST *T, int key)
{
    int *np = (int *) malloc(sizeof(int));
    *np = -2*key;
    bst_insert(T, 2*key, np);
    //printf("inserted %d\n", 2*key);
    //if (SearchPolicy == AVL)
    //	printf("  rotations %d\n", bst_rotations(T));
}

/* build a complete tree with the lowest level full.  The tree has 
 * 2^(levels)-1 nodes and is perfectly balanced.  The key range is twice the 
 * size of the tree and only the even keys are inserted in the tree.
 * 
 * T - tree to insert into
 * levels - number of levels in the tree
 */
void build_optimal(BST *T, int levels)
{
    int n, start, inc, i, j;
    n = pow(2, levels);
    for (i = 0; i<levels; i++) {
        start = n/pow(2, i + 1); inc = 2 * start;
        for (j = start; j < n; j += inc) {
	    build_one_node(T, j);
        }
    }
}

/* build a random tree with n nodes.  The nodes are numbered 2 to 2n and are
 * inserted randomly with a uniform distribution.  Only the even keys are
 * inserted.  Use Knuth shuffle to create random permutation.
 *
 * T - tree to insert into
 * nodes - number of nodes to add
 */
void build_random(BST *T, int nodes)
{
    int *narray;
    int i, key, temp;
    narray = (int *) malloc(nodes*sizeof(int));
    for (i = 0; i<nodes; i++)
        narray[i] = i;
    for (i = 0; i<nodes; i++) {
        key = (int) (drand48() * (nodes - i)) + i;
        assert(i <= key && key < nodes);
        temp = narray[i]; narray[i] = narray[key]; narray[key] = temp;
	build_one_node(T, narray[i]+1);
    }
    free(narray);
}

/* build a tree with 2^(levels)-1 nodes.  The nodes are numbered 2 to 2n and are
 * inserted in a poor order.  Only the even keys are inserted.  
 * 
 * The number of sets is sets
 * The size of a set is set_size
 *
 * Keys in a set are ascending, decending, or zig-zag
 *
 * T - tree to insert into
 * levels - used to calcuate the nubmer of nodes
 */
void build_poor(BST *T, int levels)
{
    int n, start, inc, i, j;
    int low, high;
    n = pow(2, levels);
    for (i = 0; i<levels/2; i++) {
        start = n/pow(2, i + 1); inc = 2 * start;
        for (j = start; j < n; j += inc) {
	    build_one_node(T, j);
        }
    }
    int sets = pow(2, levels/2);
    int set_size = pow(2, levels - levels/2);
    for (i = 0; i < sets; i++) {
	low = i*set_size + 1; high = (i+1)*set_size - 1;
	int type = (int) 4*drand48();
	// randomly select type of insertion pattern
	if (type == 0) {  // ascending
	    for (j = low; j <= high; j++) {
		build_one_node(T, j);
	    }
	} else if (type == 1) {  // decending
	    for (j = high; j >= low; j--) {
		build_one_node(T, j);
	    }
	} else if (type == 2) {  // zig-zag, low first
	    for (j = 0; j < (set_size-1)/2; j++) {
		build_one_node(T, low + j);
		build_one_node(T, high - j);
	    }
	    if (j*2 != set_size-1) {
		build_one_node(T, low + j);
	    }
	} else {  // zig-zag, high first
	    for (j = 0; j < (set_size-1)/2; j++) {
		build_one_node(T, high - j);
		build_one_node(T, low + j);
	    }
	    if (j*2 != set_size-1) {
		build_one_node(T, low + j);
	    }
	}
    }
}



/* read in command line arguments and store in global variables for easy
 * access by other functions.
 *
 * argc - argument count
 * argv - argument string array
 *
 */
void getCommandLine(int argc, char **argv)
{
    /* optopt--if an unknown option character is found
     * optind--index of next element in argv 
     * optarg--argument for option that requires argument 
     * "x:" colon after x means argument required
     */
    int c;
    int index;

    while ((c = getopt(argc, argv, "w:t:s:f:veoru:pq")) != -1)
        switch(c) {
            case 'w': Levels = atoi(optarg);       break;
            case 't': Trials = atoi(optarg);       break;
            case 's': Seed = atoi(optarg);         break;
            case 'v': Verbose = TRUE;              break;
            case 'e': EquilibriumTest = TRUE;      break;
            case 'o': OptimalTest = TRUE;          break;
            case 'r': RandomTest = TRUE;           break;
            case 'p': PoorTest = TRUE;             break;
            //case 'q': WorstTest = TRUE;            break;
            case 'u': UnitNumber = atoi(optarg);   break;
            case 'f':
                if (strcmp(optarg, "bst") == 0)
                    SearchPolicy = BS_TREE;
                else if (strcmp(optarg, "avl") == 0)
                    SearchPolicy = AVL;
                //else if (strcmp(optarg, "23t") == 0)
                //    SearchPolicy = TWOTHREET;
                else {
                    fprintf(stderr, "invalid search policy: %s\n", optarg);
                    exit(1);
                }
                break;
            case '?':
                if (isprint(optopt))
                    fprintf(stderr, "Unknown option %c.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            default:
                printf("Lab5 command line options\n");
                printf("General options ---------\n");
                printf("  -v        turn on verbose prints (default off)\n");
                printf("  -s 123    seed for random number generator\n");
                //printf("  -f bst|avl|23t\n");
                printf("  -f bst|avl\n");
                printf("            Type of tree\n");
                printf("  -u 0      run unit test driver with custom tree\n");
                printf("  -e        run equilibrium test driver\n");
                printf("  -o        run access test driver with optimum tree\n");
                printf("  -r        run access test driver with random tree\n");
                printf("  -p        run driver with poor insertion order\n");
                printf("\nOptions for test driver ---------\n");
                printf("  -w 16     levels in tree for drivers\n");
                printf("  -t 50000  number of trials in drivers\n");
                exit(1);
        }
    for (index = optind; index < argc; index++)
        printf("Non-option argument %s\n", argv[index]);
}

