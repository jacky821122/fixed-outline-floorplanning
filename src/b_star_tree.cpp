#include "b_star_tree.hpp"

using namespace std;

int BStarTree::GetRootNodeIdx() const { return root_idx_; }

int BStarTree::GetNumNodes() const { return nodes_.size(); }

int BStarTree::GetNodeMacroInstanceIdx(int idx) const {
  return nodes_.at(idx).macro_instance_idx_;
}

int BStarTree::GetNodeParentIdx(int idx) const {
  return nodes_.at(idx).parent_idx_;
}

int BStarTree::GetNodeLeftChildIdx(int idx) const {
  return nodes_.at(idx).left_child_idx_;
}

int BStarTree::GetNodeRightChildIdx(int idx) const {
  return nodes_.at(idx).right_child_idx_;
}

int BStarTree::CalculateHeight() const { return Height(root_idx_); }

int BStarTree::AddNewNode(int macro_instance_idx) {
  int idx = nodes_.size();

  nodes_.push_back(Node(macro_instance_idx));

  return idx;
}

void BStarTree::Skew() {
  if (!nodes_.empty()) {
    root_idx_ = 0;
    for (int i = 0; i < nodes_.size() - 1; ++i) {
      nodes_[i].left_child_idx_ = i + 1;
    }
    for (int i = 1; i < nodes_.size(); ++i) {
      nodes_[i].parent_idx_ = i - 1;
    }
  }
}

void BStarTree::DeleteNode(int idx) {
  Node& node = nodes_.at(idx);
  int parent_idx = node.parent_idx_;
  int left_child_idx = node.left_child_idx_;
  int right_child_idx = node.right_child_idx_;

  int child_idx = -1;
  if (left_child_idx != -1 && right_child_idx != -1) {
    child_idx = left_child_idx;

    int current_idx = child_idx;
    while (nodes_.at(current_idx).left_child_idx_ != -1 &&
           nodes_.at(current_idx).right_child_idx_ != -1) {
      current_idx = nodes_.at(current_idx).left_child_idx_;
    }
    Node& current_node = nodes_.at(current_idx);
    if (current_node.right_child_idx_ != -1) {
      current_node.left_child_idx_ = current_node.right_child_idx_;
      current_node.right_child_idx_ = -1;
    }

    while (current_idx != idx) {
      Node& current_node = nodes_.at(current_idx);
      Node& current_node_parent = nodes_.at(current_node.parent_idx_);
      Node& current_node_parent_right_child =
          nodes_.at(current_node_parent.right_child_idx_);
      current_node.right_child_idx_ = current_node_parent.right_child_idx_;
      current_node_parent_right_child.parent_idx_ = current_idx;
      current_idx = current_node.parent_idx_;
    }
  } else if (left_child_idx != -1) {
    child_idx = left_child_idx;
  } else if (right_child_idx != -1) {
    child_idx = right_child_idx;
  }

  if (child_idx != -1) {
    nodes_.at(child_idx).parent_idx_ = parent_idx;
  }

  if (parent_idx == -1) {
    root_idx_ = child_idx;
  } else {
    Node& parent = nodes_.at(parent_idx);
    if (parent.left_child_idx_ == idx) {
      parent.left_child_idx_ = child_idx;
    } else {
      parent.right_child_idx_ = child_idx;
    }
  }

  node.parent_idx_ = -1;
  node.left_child_idx_ = -1;
  node.right_child_idx_ = -1;
}

void BStarTree::InsertNode(int idx, int target_node_idx, char position) {
  Node& node = nodes_.at(idx);
  Node& target_node = nodes_.at(target_node_idx);

  node.parent_idx_ = target_node_idx;
  switch (position) {
    case 'L': {
      node.left_child_idx_ = target_node.left_child_idx_;
      if (node.left_child_idx_ != -1) {
        nodes_.at(node.left_child_idx_).parent_idx_ = idx;
      }
      target_node.left_child_idx_ = idx;
      break;
    }
    case 'R': {
      node.right_child_idx_ = target_node.right_child_idx_;
      if (node.right_child_idx_ != -1) {
        nodes_.at(node.right_child_idx_).parent_idx_ = idx;
      }
      target_node.right_child_idx_ = idx;
      break;
    }
    default:
      break;
  }
}

void BStarTree::SwapNodes(int idx_a, int idx_b) {
  Node& node_a = nodes_.at(idx_a);
  Node& node_b = nodes_.at(idx_b);

  int node_a_parent_idx = node_a.parent_idx_;
  if (node_a_parent_idx == -1) {
    root_idx_ = idx_b;
  } else {
    Node& node_a_parent = nodes_.at(node_a_parent_idx);
    if (node_a_parent.left_child_idx_ == idx_a) {
      node_a_parent.left_child_idx_ = idx_b;
    } else {
      node_a_parent.right_child_idx_ = idx_b;
    }
  }
  int node_b_parent_idx = node_b.parent_idx_;
  if (node_b_parent_idx == -1) {
    root_idx_ = idx_a;
  } else {
    Node& node_b_parent = nodes_.at(node_b_parent_idx);
    if (node_b_parent.left_child_idx_ == idx_b) {
      node_b_parent.left_child_idx_ = idx_a;
    } else {
      node_b_parent.right_child_idx_ = idx_a;
    }
  }

  int tmp_parent_idx = node_a.parent_idx_;
  int tmp_left_child_idx = node_a.left_child_idx_;
  int tmp_right_child_idx = node_a.right_child_idx_;
  node_a.parent_idx_ = node_b.parent_idx_;
  node_a.left_child_idx_ = node_b.left_child_idx_;
  node_a.right_child_idx_ = node_b.right_child_idx_;
  node_b.parent_idx_ = tmp_parent_idx;
  node_b.left_child_idx_ = tmp_left_child_idx;
  node_b.right_child_idx_ = tmp_right_child_idx;

  int node_a_left_child_idx = node_a.left_child_idx_;
  if (node_a_left_child_idx != -1) {
    nodes_.at(node_a_left_child_idx).parent_idx_ = idx_a;
  }
  int node_a_right_child_idx = node_a.right_child_idx_;
  if (node_a_right_child_idx != -1) {
    nodes_.at(node_a_right_child_idx).parent_idx_ = idx_a;
  }
  int node_b_left_child_idx = node_b.left_child_idx_;
  if (node_b_left_child_idx != -1) {
    nodes_.at(node_b_left_child_idx).parent_idx_ = idx_b;
  }
  int node_b_right_child_idx = node_b.right_child_idx_;
  if (node_b_right_child_idx != -1) {
    nodes_.at(node_b_right_child_idx).parent_idx_ = idx_b;
  }
}

int BStarTree::Height(int root_idx) const {
  if (root_idx == -1) {
    return 0;
  }

  int left_child_idx = nodes_.at(root_idx).left_child_idx_;
  int right_child_idx = nodes_.at(root_idx).right_child_idx_;

  return 1 + max(Height(left_child_idx), Height(right_child_idx));
}

ostream& operator<<(ostream& os, const BStarTree& b_star_tree) {
  const int indent = 2;
  cout << string(indent, ' ') << "root_idx_: " << b_star_tree.GetRootNodeIdx()
       << endl;
  cout << string(indent, ' ') << "nodes_:" << endl;
  for (int i = 0; i < b_star_tree.GetNumNodes(); ++i) {
    cout << string(indent * 2, ' ') << "Node: " << i << endl;
    cout << string(indent * 3, ' ')
         << "macro_instance_idx_: " << b_star_tree.GetNodeMacroInstanceIdx(i)
         << endl;
    cout << string(indent * 3, ' ')
         << "parent_idx_: " << b_star_tree.GetNodeParentIdx(i) << endl;
    cout << string(indent * 3, ' ')
         << "left_child_idx_: " << b_star_tree.GetNodeLeftChildIdx(i) << endl;
    cout << string(indent * 3, ' ')
         << "right_child_idx_: " << b_star_tree.GetNodeRightChildIdx(i) << endl;
  }

  return os;
}
