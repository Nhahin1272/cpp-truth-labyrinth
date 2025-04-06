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

    // 🎲 Randomly select one agent to be the mole
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

    
    return 0;
}
