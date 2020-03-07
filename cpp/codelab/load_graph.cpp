#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"

using namespace std;


class Model {
private:
    std::unique_ptr<tensorflow::Session> session_;
public:
    Model(const string& graph_def_filename) {
        tensorflow::GraphDef graph_def;
        TF_CHECK_OK(tensorflow::ReadTextProto(tensorflow::Env::Default(),
                                                graph_def_filename, &graph_def));
        session_.reset(tensorflow::NewSession(tensorflow::SessionOptions()));
        TF_CHECK_OK(session_->Create(graph_def));
    }

    void Init() { TF_CHECK_OK(session_->Run({}, {}, {"init"}, nullptr)); }
};

int main(int argc, char* argv[]) {
    const string graph_def_filename = "conf/graph/model";

    // Setup global state for TensorFlow.
    tensorflow::port::InitMain(argv[0], &argc, &argv);

    std::cout << "Loading graph\n";
    Model model(graph_def_filename);

    std::cout << "initing...\n";

    model.Init();

    return 0;
}