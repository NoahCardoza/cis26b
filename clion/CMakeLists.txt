cmake_minimum_required(VERSION 3.13)
project(cis26B C)

set(CMAKE_C_STANDARD 99)

# hw04
add_executable(hw04a hw04/a/26B_Hw_4A.c)
add_executable(hw04b hw04/b/26B_Hw_4B.c)

# hw05
add_executable(hw05 hw05/src/main.c hw05/lib/bst/bst.c hw05/lib/bst/bst.h hw05/lib/queue/queue.h hw05/lib/queue/queue.c hw05/src/parser.c hw05/src/parser.h)
add_executable(he05-bst-queue-tests hw05/lib/bst/bst.c hw05/lib/bst/bst.h hw05/lib/queue/queue.h hw05/lib/queue/queue.c hw05/other/bst_queue.test.c)


# add_executable(hw04b hw04/b/26B_Hw_4B.c)

# ec03
add_executable(ec03 ec03/EC_Tables.c)

# ec04
add_executable(ec04 ec04/main.c)

# ec05
add_executable(ec05a ec05/_1_va_arg_largest_1.c)
add_executable(ec05b ec05/_2_va_arg_largest_2.c)
add_executable(ec05c ec05/_3_va_arg_strings_1.c)
add_executable(ec05d ec05/_4_va_arg_avg.c)

# ex06
add_executable(ec06 ec06/main.c)

# ex07
add_executable(ec07 ec07/main.c)
add_executable(class main.c)

# scratch
add_executable(scratch /Users/noahcardoza/Library/Preferences/CLion2018.3/scratches/scratch.c)