#include "abstractsyntaxtree.h"

AbstractSyntaxTree::~AbstractSyntaxTree() {
  std::cout << "called AST" << std::endl;
}

/**
 * Calculates the value from an AST (Abstract Syntax Tree). To parse numbers from strings, please use std::stod
 * @return A double representing the calculated value from the expression transformed into an AST
 */
double AbstractSyntaxTree::eval() const {
  return recursiveEval(getRoot());
}

double AbstractSyntaxTree::recursiveEval(Node * subroot) const {
  if (subroot->left != NULL && subroot->left->left == NULL && subroot->right->left == NULL) {
    return mathStep(subroot);
  } else if (subroot->left->left == NULL && subroot->right->left != NULL)  {
    double known = std::stod(subroot->left->elem);
    return oneUnknownMath(subroot, subroot->right, known);
  } else if (subroot->right->left == NULL && subroot->left->left != NULL)  {
    double known = std::stod(subroot->right->elem);
    return oneUnknownMath(subroot, subroot->left, known);
  } else {
    return twoUnknownMath(subroot, subroot->left, subroot->right);
  }
}

double AbstractSyntaxTree::twoUnknownMath(Node * root, Node * leftNode, Node * rightNode) const {
  if (root->elem == "+") {
    return recursiveEval(leftNode) + recursiveEval(rightNode);
  } else if (root->elem == "-") {
    return recursiveEval(leftNode) - recursiveEval(rightNode);
  } else if (root->elem == "*") {
    return recursiveEval(leftNode) * recursiveEval(rightNode);
  } else {
    return recursiveEval(leftNode) / recursiveEval(rightNode);
  }
}

double AbstractSyntaxTree::oneUnknownMath(Node * root, Node * unknownNode, double known) const {
  if (root->elem == "+") {
    return known + recursiveEval(unknownNode);
  } else if (root->elem == "-") {
    return known - recursiveEval(unknownNode);
  } else if (root->elem == "*") {
    return known * recursiveEval(unknownNode);
  } else {
    return known / recursiveEval(unknownNode);
  }
}

double AbstractSyntaxTree::mathStep(Node * subroot) const {
  if (subroot->elem == "+") {
    return std::stod(subroot->left->elem) + std::stod(subroot->right->elem);
  } else if (subroot->elem == "-") {
    return std::stod(subroot->left->elem) - std::stod(subroot->right->elem);;
  } else if (subroot->elem == "*") {
    return std::stod(subroot->left->elem) * std::stod(subroot->right->elem);;
  } else {
    return std::stod(subroot->left->elem) / std::stod(subroot->right->elem);;
  }
}
