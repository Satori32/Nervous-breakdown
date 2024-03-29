#pragma once
#include <memory>
#include <deque>
#include <cstdint>
#include <tuple>
#include <random>
#include <string>
//#include <functional>

class ConcentrationSystem {
	typedef std::tuple<bool, bool, std::uintmax_t> Card;//open,onceopen,number.
public:
	class IConcentrationPlayer;
	typedef std::deque<Card> CardSet;
	typedef std::shared_ptr<IConcentrationPlayer> SharedPlayer;
protected:

	typedef std::tuple<SharedPlayer, std::intmax_t> Data;

public:
	bool PushPlayer(SharedPlayer Sp) {
		Players.push_back({ Sp ,0 });
		return true;
	}

	bool Initialize(const std::size_t& CardCount) {
		Clear();
		CreateCardSet(CardCount);
		return true;

	}
	bool CreateCardSet(const std::size_t& CardCount) {

		for (std::size_t i = 1; i < CardCount; i++) {
			Cards.push_back({ false,false,i });
			Cards.push_back({ false,false,i });
		}
		std::shuffle(Cards.begin(), Cards.end(), mr);
		std::shuffle(Cards.begin(), Cards.end(), mr);
		std::shuffle(Cards.begin(), Cards.end(), mr);
		return true;
	}
	bool ShufflePlayer() {
		std::shuffle(Players.begin(), Players.end(), mr);
		return true;
	}
	bool Clear() {
		Cards.clear();
		Players.clear();
		return true;
	}
	bool IsEnd() {
		for (auto& o : Cards) {
			if (std::get<0>(o) == false)return false;
		}

		return true;
	}
	bool IsCorrect(Card& A, Card& B) {
		if (&A == &B) return false;
		if (std::get<0>(A)) return false;
		if (std::get<0>(B)) return false;

		if (std::get<2>(A) == std::get<2>(B)) {
			std::get<0>(A) = true;
			std::get<0>(B) = true;
		}
		else {
			return false;
		}
		return true;
	}

	bool Game() {

		for (std::size_t i = 0; i < Players.size();i++) {
			std::size_t XX = i;
			std::cout << std::get<0>(Players[i])->Name() << " Turn!" << std::endl;
			std::deque<Card> T;
			for (auto& oo : Cards) {
				if (std::get<1>(oo) != false) {
					T.push_back(oo);
				}
				else {
					T.push_back({ false,false,-1 });
				}
			}
			
			SA:

			std::size_t A = std::get<0>(Players[i])->ThinkFirst(T);	
			std::cout << std::get<0>(Players[i])->Name() << " First Choice "<< A << std::endl;

			if (std::get<0>(Cards[A])) {
				std::cout<< A << " is alrady open."<<std::endl;
				goto SA;
			}
			std::get<1>(Cards[A]) = true;
			T.clear();
			for (auto& oo : Cards) {
				if (std::get<1>(oo) != false) {
					T.push_back(oo);
				}
				else {
					T.push_back({ false,false,-1 });
				}
			}

			SB:

			std::size_t B = std::get<0>(Players[i])->ThinkSecond(T);	
			std::cout << std::get<0>(Players[i])->Name() << " Second Choice "<< B << std::endl;

			if (std::get<0>(Cards[B])) {
				std::cout << B << " is alrady open."<<std::endl;
				goto SB;
			}

			std::get<1>(Cards[B]) = true;
			for(std::size_t j=0;j<Players.size();j++){
			//for (auto& E : Players) {
				if (!(Players[j] == Players[i])) {
					std::get<0>(Players[j])->EnemyOpen(A, std::get<2>(Cards[A]), B, std::get<2>(Cards[B]));
				}
			}

			if (IsCorrect(Cards[A], Cards[B])) {
				std::get<1>(Players[i])++;
				std::cout << std::get<0>(Players[i])->Name() << " Find Pair!" << std::endl;
				i = XX - 1;
				bool f = true;
				if (IsEnd()) break;
			}else{
				std::cout << "MISS!!" << std::endl;
			}
			
	
		}


		return true;
	}

	bool ChooseWiner() {
		Data W = Players[0];
		if (IsEnd()) {
			for (auto& o : Players) {
				if (std::get<1>(W) < std::get<1>(o)) {
					W = o;
				}
			}
			std::cout << "Winner is " << std::get<0>(W)->Name() << std::endl;
		}
		return true;
	}

	bool Show() {
		char Ch[] = "0123456789abcdefghijklnmopqrstuvwxyz";

		for (auto& o : Cards) {
			std::cout <<'['<<(std::get<0>(o) ? Ch[std::get<2>(o)] : '*')<<']';
		}
		std::cout << std::endl;
		return true;
	}

	class IConcentrationPlayer {
	public:
		virtual std::string Name() { return "774"; }
		virtual bool Initialize() { return true; }
		virtual std::size_t ThinkFirst(const std::deque<Card>&) { return true; }
		virtual std::size_t ThinkSecond(const std::deque<Card>&) { return true; }
		virtual bool EnemyOpen(const std::size_t& IndexA, const std::intmax_t& NumberA,const std::size_t& IndexB, const std::intmax_t& NumberB) { return true; }
	};
protected:
	std::deque<Data> Players;
	std::deque<Card> Cards;
	std::random_device rd;
	std::minstd_rand mr;
};

///-----
///-----
///-----

class RandomPlayer :public ConcentrationSystem::IConcentrationPlayer {
public:
	RandomPlayer():mt(rd()){}
	RandomPlayer(const std::string  In):N(In),mt(rd()){}

	virtual std::string Name() {
		return "RandomPlayer!"+N;
	}
	virtual std::size_t ThinkFirst(const ConcentrationSystem::CardSet& In) {
		std::uniform_int_distribution<> UI(0, In.size() - 1);

		return UI(mt);
	}
	virtual std::size_t ThinkSecond(const ConcentrationSystem::CardSet& In) {
		std::uniform_int_distribution<> UI(0, In.size() - 1);

		return UI(mt);
	}
protected:
	std::random_device rd;
	std::mt19937 mt;
	std::string N;
};