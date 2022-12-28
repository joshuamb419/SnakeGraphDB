#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "Node.h"

int main(int argc, char** argv){
    std::string folder = "./db_folder/";

    
    // std::string name = "How to build MuffinKeep";
    // std::string key  = "RecipeTitle";
    // std::string value = "MuffinKeep Instructions";

    // Node node = Node(folder, 1, name);

    // std::vector<unsigned char> data(value.begin(), value.end());
    // node.set_value(key, data);

    // key = "Instructions";
    // value = "\nStep 1: Plan something wildly beyond what you're capable of\nStep 2: Decide that you also need to build a database\nStep 3: Rewrite the node structure 3 times on your own\nStep 4: Talk with someone else and they make you realize you need to do it differently\nStep 5: Cry for a little\nStep 6: hopefully finally be done with the basic node unit?\nStep 7: realize youre really not done because it \"works\" but is really not optimized";
    // data = std::vector<unsigned char>(value.begin(), value.end());
    // node.set_value(key, data);

    // key = "Random Noise";
    // value = "pharetra vel turpis nunc eget lorem dolor sed viverra ipsum nunc aliquet bibendum enim facilisis gravida neque convallis a cras semper auctor neque vitae tempus quam pellentesque nec nam aliquam sem et tortor consequat id porta nibh venenatis cras sed felis eget velit aliquet sagittis id consectetur purus ut faucibus pulvinar elementum integer enim neque volutpat ac tincidunt vitae semper quis lectus nulla at volutpat diam ut venenatis tellus in metus vulputate eu scelerisque felis imperdiet proin fermentum leo vel orci porta non pulvinar neque laoreet suspendisse interdum consectetur libero id faucibus nisl tincidunt eget nullam non nisi est sit";
    // data = std::vector<unsigned char>(value.begin(), value.end());
    // node.set_value(key, data);

    // node.write_data();

    Node *node = new Node(folder, 1);
    std::cout << node->get_name() << std::endl;
    node->load_data();
    delete node;
}