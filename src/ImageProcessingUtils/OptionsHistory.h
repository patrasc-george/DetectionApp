#include <deque>
#include "FrameOptions.h"

class IMAGEPROCESSINGUTILS_API OptionsHistory {
private:
	std::deque<FrameOptions> undoStack;
	std::deque<FrameOptions> redoStack;
	FrameOptions currentStatus;
	short maxOptions = 10;
	revertable_options lastChangedOption;

public:
	/**
	 * @brief Adds a new option to the history.
	 * @details This function adds a new option to the history of options.
	 It first checks if the undo stack is full (i.e., has reached its maximum size of 10).
	 If it is, it removes the oldest option from the stack.
	 It then adds the current status to the undo stack and updates the current status with the provided option and value.
	 If there are any options in the redo stack, it clears them.
	 This function can be called whenever an option is changed by the user to keep track of its previous values and allow for undo/redo functionality.
	 * @param[in] prop The option to be added.
	 * @param[in] value The value of the option to be added.
	 */
	void add(revertable_options prop, short value);

	/**
	 * @brief Checks if redo is possible.
	 * @details This function checks if redo is possible by checking if the redo stack is not empty.
	 If it is not empty, it means that there are options that can be redone and it returns true.
	 Otherwise, it returns false to indicate that redo is not possible.
	 * @return Returns true if redo is possible, otherwise returns false.
	 */
	bool canRedo();

	/**
	 * @brief Checks if undo is possible.
	 * @details This function checks if undo is possible by checking if the undo stack is not empty.
	 If it is not empty, it means that there are options that can be undone and it returns true.
	 Otherwise, it returns false to indicate that undo is not possible.
	 * @return Returns true if undo is possible, otherwise returns false.
	 */
	bool canUndo();

	/**
	 * @brief Resets the history.
	 * @details This function resets the history by clearing both the undo and redo stacks.
	 It can be called whenever the history needs to be reset (e.g., when loading a new image).
	 */
	void reset();

	/**
	 * @brief Undoes the last change.
	 * @details This function undoes the last change by moving the current status from the undo stack to the redo stack
	 and setting the current status to the previous status stored in the undo stack.
	 It can be called whenever an undo operation is requested by the user (e.g., when they click on an undo button).
	 */
	void undo();

	/**
	 * @brief Redoes the last change.
	 * @details This function redoes the last change by moving the current status from the redo stack to the undo stack
	 and setting the current status to the previous status stored in the redo stack.
	 It can be called whenever a redo operation is requested by the user (e.g., when they click on a redo button).
	 */
	void redo();

	/**
	 * @brief Gets the current status.
	 * @details This function returns a pointer to the current status.
	 It can be called at any time to get a reference to the current status and access its properties or methods.
	 * @return Returns a pointer to the current status.
	 */
	FrameOptions* get();

	/**
	 * @brief Returns a string describing the last changed option.
	 * @details This function returns a string describing the last changed option.
	 It can be called at any time to get a human-readable description of the last option that was changed by the user.
	 * @return A string describing the last changed option.
	 */
	std::string lastChange();
};