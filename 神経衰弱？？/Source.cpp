#include <iostream>
#include <memory>
#include "SinkeiSuizyakuSystem.h"

int main() {
	NervousBreakdownSystem NBS;
	NervousBreakdownSystem::SharedPlayer A = std::make_shared<RandomPlayer>("1");
	NervousBreakdownSystem::SharedPlayer B = std::make_shared<RandomPlayer>("2");

	std::size_t  C = 9;
	//std::size_t  C = 4;
	NBS.Initialize(C);

	NBS.PushPlayer(A);
	NBS.PushPlayer(B);
	while (!NBS.IsEnd()) {
		NBS.Game();
		NBS.Show();
	}
	NBS.ChooseWiner();

	return 0;
}