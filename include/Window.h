#pragma once
#include <stdbool.h>
#include <windows.h>
// This needs to be here. Seems to conflict with windows.h
#include <commctrl.h>

#ifdef __cplusplus
extern "C"
{
#endif
    // Value to auto size controls.
    static const int AUTO_SIZE = -1;

    /// @brief Window struct.
    typedef struct Window Window;

    /// @brief Event function definition.
    typedef void (*EventFunction)(Window *, WPARAM, void *);

    /// @brief Creates a new window.
    /// @param windowClass Window class.
    /// @param title Window title.
    /// @param width Width of the window.
    /// @param height Height of the window.
    /// @param windowColor Background color of window
    /// @param handle Application handle.
    /// @return New window on success. NULL on failure.
    /// @note The window and all its children will automatically be freed when the message WM_QUIT is posted to it.
    Window *windowCreate(const char *windowClass,
                         const char *title,
                         int width,
                         int height,
                         COLORREF windowColor,
                         HINSTANCE handle);

    /// @brief Creates and sets the system font.
    /// @param window Window to set the font for.
    /// @param font Name of the font.
    /// @param size Size of the font.
    void windowSetFont(Window *window, const char *fontName, int size);

    /// @brief Sets the color used for drawing text.
    /// @param window Window to set color for.
    /// @param color Color to set text to.
    void windowSetTextColor(Window *window, COLORREF color);

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
    /// @param style Style of EDIT control.
    /// @param eventFunction Function that is called for an event.
    /// @return Handle of the window/child on success. NULL on failure.
    Window *windowAddTextInput(Window *window,
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
    /// @param height Height of the text input. AUTO_SIZE is not valid for this control.
    /// @param eventFunction Function to be executed when an event occurs with the input.
    /// @param data Data pointer passed to the function above.
    /// @return Handle of the window/child on success. NULL on failure.
    Window *windowAddMultilineTextInput(Window *window,
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
    /// @return Handle of the window/child on success. NULL on failure.
    Window *windowAddPasswordInput(Window *window, int x, int y, int width, EventFunction eventFunction, void *data);

    /// @brief Adds a button to the
    /// @param window Window to add the button to.
    /// @param x X coordinate of the window.
    /// @param y Y coordinate of the window.
    /// @param width Width of the button.
    /// @param height Height of the button. Passing AUTO_HEIGHT will size this automatically according to font size.
    /// @param buttonText Text displayed on button.
    /// @param style Style of button to use.
    /// @param eventFunction Function to execute on button triggering and event.
    /// @return Handle of the window/child on success. NULL on failure.
    Window *windowAddButton(Window *window,
                            int x,
                            int y,
                            int width,
                            int height,
                            const char *buttonText,
                            DWORD style,
                            EventFunction eventFunction,
                            void *data);

    /// @brief Adds a combobox to the window.
    /// @param window Window to add combo box to.
    /// @param x X coordinate
    /// @param y Y coordinate
    /// @param width Width of box.
    /// @param height Height of box. AUTO_SIZE is not valid for this.
    /// @param eventFunction Function executed on event.
    /// @param data Data to pass to eventFunction.
    /// @return Window pointer on success. NULL on failure.1
    Window *windowAddComboxBox(Window *window,
                               int x,
                               int y,
                               int width,
                               int height,
                               DWORD style,
                               EventFunction eventFunction,
                               void *data);

    /// @brief Adds a progres bar to the window.
    /// @param window Window to add the bar to.
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @param width Width.
    /// @param height Height.
    /// @param style Progress bar style.
    /// @return Progress bar window.
    Window *windowAddProgressBar(Window *window, int x, int y, int width, int height, DWORD style);

    /// @brief Returns the window handle.
    /// @param window Window to get the handle of.
    /// @return Window's handle?
    HWND windowGetHandle(Window *window);

    /// @brief Retrieves the text from a window.
    /// @param window Window of input.
    /// @param id ID of the input.
    /// @param buffer Buffer to write text to.
    /// @param bufferSize Size of the buffer to write text to.
    /// @return True on success. False on failure or buffer is too small.
    bool windowGetText(Window *window, char *buffer, size_t bufferSize);

    /// @brief Sets the text of a window.
    /// @param window Window owning text input.
    /// @param id The id of the input to set.
    /// @param text Text to set the input to.
    /// @return Handle of the window/child on success. NULL on failure.
    bool windowSetText(Window *window, const char *text);

    /// @brief Appends text to the input with id.
    /// @param window Window owning the text input.
    /// @param id ID of the input to append to.
    /// @param text Text to append.
    /// @return Handle of the window/child on success. NULL on failure.
    void windowAppendText(Window *window, const char *text);

#ifdef __cplusplus
}
#endif
