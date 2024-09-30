# as2micro

This is the repository for the microcontroller code for Assignment 2.

## Development

The following is the correct way to develop on this repository, and it must be
followed.

1. Create a branch: In VSCode click on the current branch in the bottom left
   corner, by default, this is `(tree icon) main`. Then select the branch that
   you are going to work in, or create the branch that you are going to work in
2. Ensure that you are in the right branch, if you are not, you will break alot
   of things, and I will just discard whatever you have done.
3. Commit often, make sure you are commiting changes often, at least within
   every ~50-100 lines changed.
4. Merging back into `main`. As you are working in your branch, you will open a
   pull request on the Github website, it will prompt you to open a pull
   request. Here we will review code changes and merge them into the main branch

It is easiest just to use VSCode's git side panel to do commits and such, it is
wise to limit commits to 1-2 files.

I have configured this repo such that you have to do this developement pattern,
you are not able to force commit to the main branch.

### Code structure

We are following the general patterns from the CC3501-RP2040-Labs repo, we are
also using the dedicated `.clang-format` file in the root directory to keep
formatting consistent throughout the project. Please don't change it, I think
that it is fairly reasonable overall.