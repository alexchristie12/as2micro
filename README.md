# as2micro

This is the main repository for the RP2040-side implementation of the CC3501
Assignment 2. This project is a pure C implementation, rather than C++.

## Why C?

Rather than using C++, C has been selected as the language of choice for this
projects. The reasoning is as follows:

- Industry standard: C is still the industry standard in embedded systems,
  having an majority share in the market. C++ has not grown in adoption much
  over the ~30 years that it has been around. I can get a source for this.
- Better interop with existing code/libraries: The vast majority of SDKs and
  libraries are written in C, so we can better guarantee compatibility, the
  design choices made in C also don't always translate well into C++
- Smaller binary: In testing it was found that C++ (especially the standard
  library) add alot to the overall binary size. The equivalent C version was
  only ~100kB, whilst the C++ version was over 600kB.
- Reduced complexity: While OOP makes sense for a number of applications, the
  imperative style of C has been found to mesh better with peripherals than
  C++. C++ OOP seems to have a lot of hidden abstraction, which is undesirable.
  We want abstraction in our **implementation** not in our language.

## Development

For this project we are going to strictly follow the the proceding development
model. The main branch is write protected, so you can't commit to it even if
you wanted to.

1. **Create a branch**. In VSCode, click on the current branch in the bottom
   left of the application, there you can select another branch to develop in,
   and/or create a new branch to develop a new feature in.
2. **Commit to the branch**. When you change things, you should be incrementally
   commiting changes, as a very rough guide, every 30-80 lines of code. Make
   your commit messages meaningful!
3. **Create a pull request**: In Github, you can create a pull request to merge
   your branch into the main branch (or whatever branch your branch was made
   from). Here we can review the code and make any required changes.
4. **Merge the branch**: In Github, we will then merge the branch after review.
   After it is merged, you can delete the branch and move onto the next branch.

When do I make a branch?
- As I have write protected the `main` branch, you will need to make a branch
  for every single feature/fix you want to add. So, I suggest adding in a
  feature, and doing it really well the first time. So you might have a `config`
  branch where we implement the config feature, or a `water-moisture` branch for
  the water moisture sensor implementation, etc.

### Style

In this project I am using the `.clang-format` format file form the QMK project,
which is a great example of a very large embedded project, make to work with
many different peripherals, all on a RP2040 (for content the project is over
1.8 million lines!).

This formatting will be alright for our needs, just accept the formatting and
move on.

### Examples

In the code that I have ripped from my labs, you will notice the use of macros
to define dynamic program behaviour, depending on what we want to debug, log
or run as a task. For example

```C
void update_leds(led_data* ld) {
    for (int i = 0; i <= NUMBER_OF_LEDS - 1; i++) {
        uint32_t led_bit_mask = ((ld->reds[i] << 24) | (ld->greens[i] << 16) | (ld->blues[i]) << 8);
        pio_sm_put_blocking(LED_PIO, 0, led_bit_mask);
    }

#ifdef DBG_LED
    log_dbg(LOG_INFORMATION, "updated LEDs");
#endif
}
```

Where we will log that we have updated the LED if `DBG_LED` is defined in 
`config.h`. Another example is seen in the way tasks are handled, to determine
what tasks the program will compile with:

```C
static tasks task_list[] = {
    TASK_IDLE,
#ifdef LED_TASK
    TASK_LED,
#endif
#ifdef ACCEL_TASK
    TASK_ACCEL,
#endif
#ifdef BLUETOOTH_TASK
    TASK_BLUETOOTH,
#endif
};
```

Where our tasks array will have a certain number of elements depending on what
we define in `config.h`

### Other general notes

As a general note, when we are writing something, don't try to be too fancy or
overcomplicate things. Most of the time the most straightforward way of
approaching things works the best, ChatGPT is not good at this...

Also think about where you are putting things and ensure that you are putting
code in the right file. Ensure that you are not doing too much in a single
function, break stuff down, within reason of course.

## Where to put stuff

In the header files, we put required `#include` statements, and the `#define`
statements in that order. Asides from that the only other things that we should
place the struct and enum definitions, and the function prototypes.

In the C source files, we put all the function implementations, and any global
variables. If we don't want to export our functions or variables, put the 
`static` keyword infront.

## Documentation

The Doxygen comments should go in the header files, above the function
prototypes, refer to `src/drivers/config_load/config_loader.h` for reference.