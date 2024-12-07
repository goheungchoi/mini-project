/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <atomic>
#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>

// Function to generate a random word of a given length
std::string GenerateRandomWord(size_t length)
{
  static const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
  static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr))); // Random number generator
  std::uniform_int_distribution<size_t> dist(0, alphabet.size() - 1);

  std::string word;
  for (size_t i = 0; i < length; ++i)
  {
    word += alphabet[dist(rng)];
  }
  return word;
}

// Function to push random words to a queue
void PushRandomWords(std::vector<std::string>& wordQueue)
{
  static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr))); // Random number generator
  std::uniform_int_distribution<int> countDist(1000, 2000);               // Random range for push count
  std::uniform_int_distribution<size_t> lengthDist(80, 100);              // Random range for word length

  int pushCount = countDist(rng); // Generate a random number of pushes

  for (int i = 0; i < pushCount; ++i)
  {
    size_t wordLength = lengthDist(rng); // Generate a random word length
    std::string randomWord = GenerateRandomWord(wordLength);
    wordQueue.push_back(randomWord); // Push the random word to the queue
  }
}

#define NUM_FRAMES_TO_BE_SIMULATED 1000

#define QUEUE_OVERLAPS 2

std::atomic<bool> notifyFinished{false};

size_t copyCount{0};
std::atomic<size_t> copyQueueIndex{0};
std::atomic<size_t> updateQueueIndex{0};

std::mutex queueMutexes[QUEUE_OVERLAPS];
std::vector<std::string> queues[QUEUE_OVERLAPS];

std::vector<char> copyBuffer;

void CopyData()
{
  copyBuffer.clear();
  for (auto& str : queues[copyQueueIndex % QUEUE_OVERLAPS])
  {
    for (char c : str)
    {
      copyBuffer.push_back(c);
    }
  }
}

void Render()
{
  while (!notifyFinished)
  {
    {
      if (updateQueueIndex - copyQueueIndex >= 3)
        copyQueueIndex = updateQueueIndex - 1;
      std::cout << "Queue " << (copyQueueIndex % QUEUE_OVERLAPS) << ": Trying to copy data..." << std::endl;
      std::lock_guard<std::mutex> copyQueueLock(queueMutexes[copyQueueIndex % QUEUE_OVERLAPS]);
      std::cout << "Queue " << (copyQueueIndex % QUEUE_OVERLAPS) << ": Copying the data..." << std::endl;
      CopyData();
      std::cout << "Queue " << (copyQueueIndex % QUEUE_OVERLAPS) << ": Finished copying data!" << std::endl;
      copyQueueIndex++;
      copyCount++;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void Update()
{
  std::cout << "Queue " << (updateQueueIndex % QUEUE_OVERLAPS) << ": Updating the data..." << std::endl;
  queues[updateQueueIndex % QUEUE_OVERLAPS].clear();
  PushRandomWords(queues[updateQueueIndex % QUEUE_OVERLAPS]);
	std::this_thread::sleep_for(std::chrono::milliseconds(16));
  std::cout << "Queue " << (updateQueueIndex % QUEUE_OVERLAPS) << ": Finished updating data!" << std::endl;
}

int main()
{
  size_t frameCount{0};

  std::unique_lock<std::mutex> updateQueueLock(queueMutexes[updateQueueIndex % QUEUE_OVERLAPS]);

  std::thread renderThread(Render);
  std::cout << "Test Started!" << std::endl;

  while (frameCount < NUM_FRAMES_TO_BE_SIMULATED)
  {
    Update();

    updateQueueIndex++;
    updateQueueLock = std::unique_lock<std::mutex>(queueMutexes[updateQueueIndex % QUEUE_OVERLAPS]);

    frameCount++;
  }

  notifyFinished = true;
  renderThread.join();

  std::cout << "Test Finished!" << std::endl;

  std::cout << "---------- Result ----------" << std::endl;
  std::cout << "Num. Copies: " << copyCount << std::endl;
  std::cout << "Num. Updates: " << updateQueueIndex << std::endl;

  return 0;
}
