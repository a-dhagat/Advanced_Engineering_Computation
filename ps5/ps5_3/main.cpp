#include <stdlib.h>
#include <time.h>

#include "../bintreeutil/bintree.h"

int main(void)
{
	srand((int)time(nullptr));

	BinaryTree <int,int> tree;
	for(int i=0; i<100; ++i)
	{
		tree.Insert(rand()%100,0);
	}

	// Testing Last()
	auto last_function = tree.Last();
	tree.TreeToVine();
	auto tail = tree.RootNode();
	while (tail.IsNotNull())
	{
		tail = tree.Right(tail);
	}
	auto last_tree_to_vine = tail;

	if (last_function != last_tree_to_vine)
		return 1;

	// Testing FindPrev()
	auto elem_find_prev = tree.FindPrev(tail);
	auto elem_tree_to_vine = tree.Up(tail);

	if (elem_find_prev != elem_tree_to_vine)
		return 1;
	
	return 0;
}