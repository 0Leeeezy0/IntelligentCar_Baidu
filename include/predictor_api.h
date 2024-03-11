#ifndef PPNC_PREDICTOR_API_H_
#define PPNC_PREDICTOR_API_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
/*!
 * \file predictor_api.h
 * \brief cpp api for ppnc predictor
 */
namespace tvm {
namespace runtime {
class Module;
class NDArray;
} // namespace runtime
} // namespace tvm

namespace picojson{
    class value;
}
using Module = tvm::runtime::Module;
using NDArray = tvm::runtime::NDArray;
using json = picojson::value;
struct NDTensor {
    std::shared_ptr<NDArray> data;
    std::vector<int64_t> shape;
    NDTensor() = default;
    NDTensor(const std::vector<int64_t> &t_shape);
    NDTensor(NDArray rhs);
    int64_t Numel();
    void copy_from_bytes(void* from, int nbytes);
    float *value()const;
};
namespace ppnc {
namespace api {
struct PPNCPredictor {
    std::string mode;
    std::string model_dir;
    std::string model_file;
    std::unique_ptr<Module> model;
    PPNCPredictor(const std::string &dir);
    ~PPNCPredictor();
    void load();
    void set_inputs(const std::unordered_map<std::string, NDTensor> &map_inputs);
    void set_inputs_zero_copy(const std::unordered_map<std::string, NDTensor> &map_inputs);
    std::vector<NDTensor> get_outputs();
    NDTensor get_output(int index);
    void run();
};
} // namespace api
} // namespace ppnc

using PPNCPredictor = ppnc::api::PPNCPredictor;
#endif // PPNC_PREDICTOR_API_H_
