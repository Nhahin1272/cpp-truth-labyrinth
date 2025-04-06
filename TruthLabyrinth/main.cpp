#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>    // For rand and srand
#include <ctime>      // For time
#include <limits.h>   // For PATH_MAX
#include <unistd.h>   // For getcwd

using namespace std;

struct Agent {
    string name;
    string statement;
    bool isMole = false;  // Flag to mark if this agent is the mole
};

int main() {
    cout << "🕵️‍♂️ Welcome to FIND THE MOLE!" << endl;
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

    // Randomly select one agent to be the mole
    srand(static_cast<unsigned int>(time(0)));  // Seed the random generator
    int moleIndex = rand() % agents.size();     // Random index
    agents[moleIndex].isMole = true;

    cout << "✅ Loaded " << agents.size() << " agents. Mole selected (hidden)." << endl;

    // 🧪 Debug print (temporary)
    cout << "\n🔍 DEBUG: The mole is " << agents[moleIndex].name << "!\n";

    // debug: Print how many agents were loaded
   // cout << "✅ Loaded " << agents.size() << " agents." << endl;

    // 🧾 Show the list of agent names for the player to choose from
    cout << "\n📜 Available Agents:\n";
    for (int i = 0; i < agents.size(); ++i) {
        cout << "  " << (i + 1) << ". " << agents[i].name << endl; // number starts from 1
    }

    // Let the player pick 3 agents to interview
    vector<int> interviewedIndices;

    for (int interview = 1; interview <= 3; ++interview) {
        int choice;
        cout << "\n🕵️ Interview #" << interview << ": Pick an agent (1-" << agents.size() << "): ";
        cin >> choice;

        // Validate input
        if (choice < 1 || choice > agents.size()) {
            cout << "❌ Invalid choice. Please pick a valid agent number.\n";
            --interview; // try again
            continue;
        }

        // Check if this agent was already interviewed
        if (find(interviewedIndices.begin(), interviewedIndices.end(), choice) != interviewedIndices.end()) {
            cout << "⚠️ You already interviewed this agent. Pick someone else.\n";
            --interview;
            continue;
        }

        // Store interview choice and show statement
        interviewedIndices.push_back(choice);
        cout << "\n🗣️ " << agents[choice - 1].name << " says: \"" << agents[choice - 1].statement << "\"\n";
    }

    
    // 🎯 Let the player guess the mole
    int guess;
    cout << "\n🎯 Final Question: Who do you think is the mole? (Enter the agent number): ";
    cin >> guess;

    // ❌ Validate guess is within the range
    if (guess < 1 || guess > agents.size()) {
        cout << "🚫 Invalid guess. Ending game.\n";
        return 1;
    }

    // 🕵️ Check if the guess was correct
    if (agents[guess - 1].isMole) {
        cout << "\n🎉 You got it! " << agents[guess - 1].name << " was the mole all along!\n";
    } else {
        // 🔍 Find who the actual mole was (to show the correct answer)
        string actualMoleName;
        for (const Agent& a : agents) {
            if (a.isMole) {
                actualMoleName = a.name;
                break;
            }
        }

        cout << "\n❌ Wrong guess! The real mole was " << actualMoleName << ".\n";
    }

    // 🏁 End of game message
    cout << "\n🧠 Thanks for playing TruthLabyrinth! Until next mission...\n";

    return 0;
}
