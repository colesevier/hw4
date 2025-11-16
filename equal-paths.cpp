#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm> // this is just to get the max function when getting the height
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int findHeight(Node* root)
{
  if (root == nullptr)
  {
    return 0;
  }
  int height1 = findHeight(root->left);
  int height2 = findHeight(root->right);
  return 1 + max(height1, height2);
}


bool equalPaths(Node * root)
{
  // Add your code below
  if (root == nullptr)
    return true;

  // sub-cases (must be recursive)
  // I think that I would go through the tree using recursion and 
  // keep checking to see if the right and left sides have the same 
  // height
  // check to see if one side is gone (secondary cases)
  if (root->left == nullptr)
  {
    return equalPaths(root->right);
  }
  else if (root->right == nullptr)
  {
    return equalPaths(root->left);
  }
  else
  {
    if (equalPaths(root->left) && equalPaths(root->right))
    {
      return (findHeight(root->left) == findHeight(root->right));
    }
    // if they are the same height but this will only check for two cases
    return false;
  }
  // final case
  //return true;
}

