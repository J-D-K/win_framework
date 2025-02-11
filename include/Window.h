#pragma once
#include <stdbool.h>
#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // End user window struct
    typedef struct Window Window;

    // Function definition.
    typedef void (*EventFunction)(Window *, WPARAM, void *);

    /// @brief Creates a new window.
    /// @param windowClass Window class.
    /// @param title Window title.
    /// @param width Width of the window.
    /// @param height Height of the window.
    /// @param font Font used for the window.
    /// @param windowColor Background color of window
    /// @param textColor Color of text drawn to the window.
    /// @param handle Application handle.
    /// @return New window on success. NULL on failure.
    /// @note The window and all its children will automatically be freed when the message WM_QUIT is posted to it.
    Window *windowCreate(const char *windowClass,
                         const char *title,
                         int width,
                         int height,
                         const char *font,
                         COLORREF windowColor,
                         COLORREF textColor,
                         HINSTANCE handle);

    /// @brief Shows the window.
    /// @param window Window to show.
    void windowShow(Window *window);

    /// @brief Runs the update routine for the window.
    /// @param window Window to update and handle.
    /// @note WM_DESTROY is not handled and PostQuitMessage is not used to allow the program to free resources correctly.
    bool windowUpdate(Window *window);

    /// @brief Adds text to be draw to the window.
    /// @param window Window to add text to.
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @param text Text to draw.
    void windowAddText(Window *window, int x, int y, const char *text);

    /// @brief Adds a text input to the window.
    /// @param window Window to add text input to.
    /// @param x X coordinate of input.
    /// @param y Y coordinate of input.
    /// @param width Width of the input box.
    /// @param background Color of the background of the input.
    /// @param readonly Whether or not the input is readonly.
    /// @param eventFunction Function that is called for an event.
    /// @return ID of the input box created. -1 on failure.
    int windowAddTextInput(Window *window,
                           int x,
                           int y,
                           int width,
                           bool readonly,
                           EventFunction eventFunction,
                           void *data);

    /// @brief Adds a multiline text input to the window.
    /// @param window Window to add text input to.
    /// @param x X coordinate of the text input.
    /// @param y Y coordinate of the text input.
    /// @param width Width of the text input.
    /// @param height Height of the text input.
    /// @param eventFunction Function to be executed when an event occurs with the input.
    /// @param data Data pointer passed to the function above.
    /// @return Input's ID on success. -1 on failure.
    int windowAddMultilineTextInput(Window *window,
                                    int x,
                                    int y,
                                    int width,
                                    int height,
                                    bool readonly,
                                    EventFunction eventFunction,
                                    void *data);

    /// @brief Adds a password style input to the window.
    /// @param window Window to add password to.
    /// @param x X coordinate of input.
    /// @param y Y coordinate of input.
    /// @param width Width of the input box.
    /// @param eventFunction Function that is called for an event.
    /// @return ID of the password box created. -1 on failure.
    int windowAddPasswordInput(Window *window, int x, int y, int width, EventFunction eventFunction, void *data);

    /// @brief Adds a button to the
    /// @param window Window to add the button to.
    /// @param x X coordinate of the window.
    /// @param y Y coordinate of the window.
    /// @param width Width of the button.
    /// @param eventFunction Function to execute on button triggering and event.
    /// @return ID of the button.
    int windowAddButton(Window *window,
                        int x,
                        int y,
                        int width,
                        const char *buttonText,
                        EventFunction eventFunction,
                        void *data);

    /// @brief Returns the main window handle.
    /// @param window Window to get the handle of.
    /// @return Window's handle?
    HWND windowGetHandle(Window *window);

    /// @brief Retrieves the text from a text or password input.
    /// @param window Window that parents the input.
    /// @param id ID of the input.
    /// @param buffer Buffer to write text to.
    /// @param bufferSize Size of the buffer to write text to.
    /// @return True on success. False on failure or buffer is too small.
    bool windowGetTextFromInput(Window *window, int id, char *buffer, size_t bufferSize);

    /// @brief Sets the text of the input.
    /// @param window Window owning text input.
    /// @param id The id of the input to set.
    /// @param text Text to set the input to.
    /// @return True on success. False on failure.
    bool windowSetInputText(Window *window, int id, const char *text);

    /// @brief Appends text to the input with id.
    /// @param window Window owning the text input.
    /// @param id ID of the input to append to.
    /// @param text Text to append.
    /// @return True on success. False on failure.
    bool windowAppendInputText(Window *window, int id, const char *text);

#ifdef __cplusplus
}
#endif
