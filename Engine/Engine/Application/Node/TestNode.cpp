#include "Engine/Application/Node/TestNode.h"
#include "Engine/Application/Engine.h"
#include "Engine/Math/Math.h"
#include "Engine/Application/Node/NodeTree.h"

namespace Engine {
	TestNode::TestNode() {
		Node* node = MEMNEW(Node());
		AddChild(node);
	}
	void TestNode::OnEnteredTree() {
		INFO_MSG(String::Format("{0}: Entered tree.",GetName()).GetRawArray());
	}
	void TestNode::OnReady() {
		INFO_MSG(String::Format("{0}: Ready.", GetName()).GetRawArray());
	}
	void TestNode::OnUpdate(float delta) {
		//double elapsed = ::Engine::Engine::GetInstance()->GetTime().GetTotal();

		//if (elapsed > 3) {
		//	GetTree()->RequestStop();
		//}
	}
	void TestNode::OnExitingTree() {
		INFO_MSG(String::Format("{0}: Exiting tree.", GetName()).GetRawArray());
	}

}