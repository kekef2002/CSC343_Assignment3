/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/main.c to edit this template
 */

/* 
 * File:   main.c
 * Author: Kervin Francois
 *
 * Created on February 23, 2025, 6:17 AM
 */

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
#include <climits>

#define SIZE 20

using namespace std;

// Create a function to find the minimum in an array segment
int findMin(int arr[], int start, int end) {
    int minVal = INT_MAX;
    for (int i = start; i < end; i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}

int main() {
    int arr[SIZE];
    srand(time(0));  // Seed for random number generation

    // Fill array with random numbers between 1 and 100
    cout << "Array Elements: ";
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 100 + 1;
        cout << arr[i] << " ";
    }
    cout << endl;

    int pipefd[2]; // File descriptors for pipe
    if (pipe(pipefd) == -1) {
        cerr << "Pipe creation failed!" << endl;
        return 1;
    }

    pid_t pid = fork(); // Create child process

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    }

    if (pid == 0) { 
        // Child process
        close(pipefd[0]); // Close unused read end
        int minChild = findMin(arr, SIZE / 2, SIZE);
        write(pipefd[1], &minChild, sizeof(minChild));
        close(pipefd[1]); // Close write end after writing

        cout << "Child Process (PID: " << getpid() << ") found min in second half: " << minChild << endl;
        exit(0);
    } else {
        // Parent process
        close(pipefd[1]); // Close unused write end
        int minParent = findMin(arr, 0, SIZE / 2);
        int minChild;

        wait(NULL); // Wait for child to complete
        read(pipefd[0], &minChild, sizeof(minChild));
        close(pipefd[0]); // Close read end after reading

        cout << "Parent Process (PID: " << getpid() << ") found min in first half: " << minParent << endl;

        // Determine the overall minimum
        int minOverall = min(minParent, minChild);
        cout << "Overall Minimum: " << minOverall << endl;
    }

    return 0;
}
