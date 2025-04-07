/*
  TruthLabyrinth: Find the Mole
  Author: Nhahin Uddin
  Description: A logic game where the player interviews agents to find which one is lying.
  The game reads agent data from a file and uses C++ fundamentals like structs, loops,
  conditionals, file I/O, and vectors. Includes replay support and win/loss tracking.
  To run: Make sure 'questions.txt' is in the 'data/' folder next to the executable.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>    // For rand and srand
#include <ctime>      // For time
#include <limits.h>   // For PATH_MAX
#include <unistd.h>   // For getcwd
#include <algorithm>  // For find()

using namespace std;

struct Agent {
    string name;
    string statement;
    bool isMole = false;  // Flag to mark if this agent is the mole
};

int main() {
    int wins = 0;
    int losses = 0;
    char playAgain;

    do {
        // 🕵️ Intro
        cout << "\n🕵️‍♂️ Welcome to FIND THE MOLE!" << endl;
        cout << "One of your fellow agents is hiding a secret..." << endl;
        cout << "You have 3 interviews to figure out which agent is the mole." << endl;
        cout << "Let the game begin!" << endl << endl;

        vector<Agent> agents;
        ifstream file("data/questions.txt");

        if (!file) {
            cout << "❌ Could not open file." << endl;
            return 1;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, statement;
            if (getline(ss, name, '|') && getline(ss, statement)) {
                Agent a;
                a.name = name;
                a.statement = statement;
                agents.push_back(a);
            }
        }

        file.close();

        // Randomly select a mole
        srand(static_cast<unsigned int>(time(0)));
        int moleIndex = rand() % agents.size();
        agents[moleIndex].isMole = true;

        // Show agent names
        cout << "✅ Loaded " << agents.size() << " agents.\n";
        cout << "\n📜 Available Agents:\n";
        for (int i = 0; i < agents.size(); ++i) {
            cout << "  " << (i + 1) << ". " << agents[i].name << endl;
        }

        // Interview loop
        vector<int> interviewedIndices;
        for (int interview = 1; interview <= 3; ++interview) {
            int choice;
            cout << "\n🕵️ Interview #" << interview << ": Pick an agent (1-" << agents.size() << "): ";
            cin >> choice;

            if (choice < 1 || choice > agents.size()) {
                cout << "❌ Invalid choice. Try again.\n";
                --interview;
                continue;
            }

            if (find(interviewedIndices.begin(), interviewedIndices.end(), choice) != interviewedIndices.end()) {
                cout << "⚠️ You already interviewed this agent. Pick someone else.\n";
                --interview;
                continue;
            }

            interviewedIndices.push_back(choice);
            cout << "\n🗣️ " << agents[choice - 1].name << " says: \"" << agents[choice - 1].statement << "\"\n";
        }

        // Show agents again for final decision
        cout << "\n📜 Here are the agents again:\n";
        for (int i = 0; i < agents.size(); ++i) {
            cout << "  " << (i + 1) << ". " << agents[i].name << endl;
        }

        // Guess the mole
        int guess;
        cout << "\n🎯 Final Question: Who do you think is the mole? (Enter the agent number): ";
        cin >> guess;

        if (guess < 1 || guess > agents.size()) {
            cout << "🚫 Invalid guess. Ending game.\n";
            return 1;
        }

        if (agents[guess - 1].isMole) {
            cout << "\n🎉 You got it! " << agents[guess - 1].name << " was the mole all along!\n";
            wins++;
        } else {
            string actualMoleName;
            for (const Agent& a : agents) {
                if (a.isMole) {
                    actualMoleName = a.name;
                    break;
                }
            }
            cout << "\n❌ Wrong guess! The real mole was " << actualMoleName << ".\n";
            losses++;
        }

        cout << "\n📊 Current Score: " << wins << " wins, " << losses << " losses\n";

        cout << "\n🔁 Would you like to play again? (y/n): ";
        cin >> playAgain;

    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "\n📈 Final Score: " << wins << " wins, " << losses << " losses\n";
    cout << "👋 Thanks for playing TruthLabyrinth! Until next mission...\n";

    return 0;
}

