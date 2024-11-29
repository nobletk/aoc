#include <assert.h>

#include "functions.h"

void test_find_first_digit() {
    size_t start_pos;
    // Test case 1: Line contains mixed digits
    start_pos = 0;
    assert(find_first_digit("two1nine", &start_pos) == 2);
    // Test case 2: Line contains numerical digits
    start_pos = 0;
    assert(find_first_digit("abc123xyz", &start_pos) == 1);
    // Test case 3: Line contains only zeros
    start_pos = 0;
    assert(find_first_digit("0000000", &start_pos) == 0);
    // Test case 4: Line contains no digits
    start_pos = 0;
    assert(find_first_digit("no_digits", &start_pos) == -1);
    // Test case 5: Line contains single digit
    start_pos = 0;
    assert(find_first_digit("pqrstsixteen", &start_pos) == 6);
    printf("All tests passed for find_first_digit!\n");
}

void test_find_last_digit() {
    size_t start_pos, end_pos;

    // Test case 1: Line contains mixed digits
    const char *line1 = "two1nine";
    start_pos = 2;
    end_pos = strlen(line1) - 1;
    assert(find_last_digit(line1, &start_pos, &end_pos) == 9);

    // Test case 2: Line contains only numerical digits
    const char *line2 = "abc123xyz";
    start_pos = 4;
    end_pos = strlen(line2) - 1;
    assert(find_last_digit(line2, &start_pos, &end_pos) == 3);

    // Test case 3: Line contains only zeros
    const char *line3 = "0000000";
    start_pos = 1;
    end_pos = strlen(line3) - 1;
    assert(find_last_digit(line3, &start_pos, &end_pos) == 0);

    // Test case 4: Line contains no digits
    const char *line4 = "no_digits";
    start_pos = strlen(line4) - 1;
    end_pos = strlen(line4) - 1;
    assert(find_last_digit(line4, &start_pos, &end_pos) == -1);

    // Test case 5: Line contains one spelled digit
    const char *line5 = "pqrstsixteen";
    start_pos = 8;
    end_pos = strlen(line5) - 1;
    assert(find_last_digit(line5, &start_pos, &end_pos) == -1);

    // Test case 6: Line contains a mix of digits and spelled digits
    const char *line6 = "one234five";
    start_pos = 3;
    end_pos = strlen(line6) - 1;
    assert(find_last_digit(line6, &start_pos, &end_pos) == 5);

    printf("All tests passed for find_last_digit!\n");
}

void test_sum_first_and_last_digits() {
    // Test case 1: Line contains numerical digits
    assert(sum_first_and_last_digits("abc123xyz") == 13);
    // Test case 2: Line contains a mix of digits and spelled digits
    assert(sum_first_and_last_digits("456abc123xyz78nine") == 49);
    // Test case 3: Line contains only zeros
    assert(sum_first_and_last_digits("0000000") == 0);
    // Test case 4: Line contains no digits
    assert(sum_first_and_last_digits("no_digits") == 0);
    // Test case 5: Line contains a single digit
    assert(sum_first_and_last_digits("1single") == 11);
    printf("All tests passed for sum_first_and_last_digits!\n");
}

void test_read_file_and_sum_digits() {
    // Test case 1:
    const char *test_file = "example.txt";

    int result = read_file_and_sum_digits(test_file);
    int expected = 142;
    assert(result == expected);

    printf(
        "Test 1 passed for test_read_file_and_sum_digits! Total sum is %d.\n",
        result);

    // Test case 2:
    const char *test_file_two = "example_two.txt";

    int result_two = read_file_and_sum_digits(test_file_two);
    int expected_two = 281;
    assert(result_two == expected_two);

    printf(
        "Test 2 passed for test_read_file_and_sum_digits! Total sum is %d.\n",
        result_two);
}

int main() {
    test_find_first_digit();
    test_find_last_digit();
    test_sum_first_and_last_digits();
    test_read_file_and_sum_digits();
    return 0;
}
