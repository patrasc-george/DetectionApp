#include <chrono>

#ifdef IMAGEPROCESSINGUTILS_EXPORTS
#define IMAGEPROCESSINGUTILS_API __declspec(dllexport)
#else
#define IMAGEPROCESSINGUTILS_API __declspec(dllimport)
#endif

class IMAGEPROCESSINGUTILS_API Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	int* counter;

public:
	/**
	 * @brief Constructs a Timer object and starts the timer.
	 * @details This constructor initializes a Timer object and starts its internal timer.
	 It also stores a reference to an integer counter that will be updated with the elapsed time when the Timer object is destroyed.
	 This constructor can be called at any time to start timing an operation (e.g., when processing an image).
	 * @param[out] c An integer reference to be used as a counter. (output)
	 */
	Timer(int& c);

	/**
	 * @brief Destroys the Timer object and stops the timer.
	 * @details This destructor stops the internal timer of a Timer object and calculates its duration.
	 It then updates the integer counter that was provided when constructing the Timer object with the elapsed time (in seconds).
	 This destructor is called automatically when a Timer object goes out of scope
	 or is deleted (e.g., when an image processing operation completes).
	 */
	~Timer();
};