#pragma once
#include "lib_ecm.h"
#include <memory>
#include <vector>

using namespace std;

// DECISION TYPES
class DecisionTreeNode {
public:
    virtual ~DecisionTreeNode() = default;
    virtual void MakeDecision(Entity* owner) = 0;
};

class Decision : public DecisionTreeNode {
protected:
    shared_ptr<DecisionTreeNode> _trueNode;
    shared_ptr<DecisionTreeNode> _falseNode;
    virtual shared_ptr<DecisionTreeNode> getBranch(Entity* owner) = 0;

public:
    Decision(shared_ptr<DecisionTreeNode> trueNode, shared_ptr<DecisionTreeNode> falseNode) : _trueNode(trueNode), _falseNode(falseNode) { }

    void MakeDecision(Entity* owner) {
        getBranch(owner)->MakeDecision(owner);
    }
};

class MultiDecision : public DecisionTreeNode {
protected:
    vector<shared_ptr<DecisionTreeNode>> _childNodes;
    virtual shared_ptr<DecisionTreeNode> getBranch(Entity* owner) = 0;

public:
    MultiDecision(const vector<shared_ptr<DecisionTreeNode>>& childNodes) : _childNodes(childNodes) { }

    void MakeDecision(Entity* owner) {
        getBranch(owner)->MakeDecision(owner);
    }
};

class RandomDecision : public Decision {
protected:
    shared_ptr<DecisionTreeNode> getBranch(Entity* owner) final;

public:
    RandomDecision(shared_ptr<DecisionTreeNode> trueNode, shared_ptr<DecisionTreeNode> falseNode) : Decision(trueNode, falseNode) { }
};

class RandomMultiDecision : public MultiDecision {
protected:
    shared_ptr<DecisionTreeNode> getBranch(Entity* owner) override final;

public:
    RandomMultiDecision(const vector<shared_ptr<DecisionTreeNode>>& childNodes) : MultiDecision(childNodes) { }
};

// COMPONENT
class DecisionTreeComponent : public Component {
private:
    shared_ptr<DecisionTreeNode> _decisionTree;

public:
    void Update(double) override;
    void Render() override { }
    explicit DecisionTreeComponent(Entity* p, shared_ptr<DecisionTreeNode> decisionTree);
    DecisionTreeComponent() = delete;
};