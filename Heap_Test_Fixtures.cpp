#include <gtest\gtest.h>
#include <RingBuffer.h>
#include <Heap.h>

static void SeedRandomGen()
{
	time_t timer;
	time(&timer);
	srand((U32)timer);
}

class HeapTest : public ::testing::Test {
public:
	HeapTest() : min_hp(Continers::MIN_HEAP), max_hp(Continers::MAX_HEAP), numElems(9000) {}
protected:
	virtual void SetUp() {
		SeedRandomGen();
	}

	// virtual void TearDown() {}

	Continers::Heap<U32> min_hp, max_hp;
	U64 numElems;
};

TEST_F(HeapTest, MinHeapPushTest) {
	using namespace std;
	for (U64 i = 0; i < numElems; ++i)
	{
		U32 pushed = min_hp.push(rand() % (numElems * 12));
		EXPECT_EQ(i + 1, min_hp.size());
		if (min_hp.size() > 1)
			EXPECT_LE(min_hp.top(), pushed);
	}
}

TEST_F(HeapTest, MaxHeapPushTest) {
	using namespace std;
	for (U64 i = 0; i < numElems; ++i)
	{
		U32 pushed = max_hp.push(rand() % (numElems * 12));
		EXPECT_EQ(i + 1, max_hp.size());
		if (max_hp.size() > 1)
			EXPECT_GE(max_hp.top(), pushed);
	}
}

TEST_F(HeapTest, MinHeapPopTest) {
	using namespace std;
	U32 last_popped = 0;
	for (U64 i = 0; i < numElems; ++i)
	{
		U32 popped = min_hp.pop();
		EXPECT_LE(last_popped, popped);
		last_popped = popped;
		//cout << "Popping: " << popped << endl;
		//cout << "Heap: "; min_hp.printHeap();
		//cout << endl;
	}
}

TEST_F(HeapTest, MaxHeapPopTest) {
	using namespace std;
	U32 last_popped = UINT_MAX;
	for (U64 i = 0; i < numElems; ++i)
	{
		U32 popped = max_hp.pop();
		EXPECT_GE(last_popped, popped);
		last_popped = popped;
		//cout << "Popping: " << popped << endl;
		//cout << "Heap: "; max_hp.printHeap();
		//cout << endl;
	}
}