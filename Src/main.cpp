#include <iostream>
#include "Utils/Utils.h"
#include "Model/Map.h"
#include "Readers/SrtmReader.h"
#include "Split/Processor.h"

void set_default_config(Utils::config *config);

void
run_processor(const Utils::config *config, Map *map, double proc_width, double proc_length, double proc_height,
              int i, int j, int k, bool rotate, const char *file_base_name);

int main() {
    //argument parsing
    Utils::config *config = static_cast<Utils::config *>(malloc(sizeof(Utils::config)));
    set_default_config(config);
    Utils::verify_configs(config);

    //data reading
    SrtmReader *reader = new SrtmReader(config->map_dir);
    Map map(reader);
//    map = reader->read_SRTM(config->west_border, config->north_border, config->east_border, config->south_border);

    double proc_width = fabs(config->west_border - config->east_border) / config->procs_x;
    double proc_length = fabs(config->north_border - config->south_border) / config->procs_y;
    double proc_height = config->height / config->procs_z;

    for (int i = 0; i < config->procs_x; ++i) {
        for (int j = 0; j < config->procs_y; ++j) {
            for (int k = 0; k < config->procs_z; ++k) {
//                run_processor(config, &map, proc_width, proc_length, proc_height, i, j, k, false, "test/yang");
                run_processor(config, &map, proc_width, proc_length, proc_height, i, j, k, true, "test/yin");
            }
        }
    }



    delete reader;
    delete config;
    return 0;
}

void
run_processor(const Utils::config *config, Map *map, double proc_width, double proc_length, double proc_height,
              int i, int j, int k, bool rotate, const char *file_base_name) {
    Processor processor(std::pair(config->west_border + i * proc_width,
                                  config->west_border + (i + 1) * proc_width),
                        std::pair(config->south_border + j * proc_length,
                                  config->south_border + (j + 1) * proc_length),
                        std::pair(k * proc_height, (k + 1) * proc_height),
                        i, j, k,
                        config->point_per_proc_x, config->point_per_proc_y, config->point_per_proc_z,
                        map, rotate);
    processor.calculate(file_base_name);
}


void set_default_config(Utils::config *config) {
    config->tolerance = 0.00001;
    config->requested_size = SIZE_MAX;
    config->output_filename = "out/krakow_tol5_iter14";
    config->input_filename = "Examples/test1.asc";
    config->read_from_ASC = false;
    config->west_border = 19.7;
    config->north_border = 50.2;
    config->east_border = 19.9;
    config->south_border = 50.05;
    config->map_dir = "Data";
    config->use_inp = false;
    config->use_smesh = false;
    config->post_utm = false;
    config->pre_utm = false;
    config->use_height = false;
    config->use_floater = false;
    config->procs_x = 10;
    config->procs_y = 10;
    config->procs_z = 10;
    config->height = 3000.;
    config->point_per_proc_x = 11;
    config->point_per_proc_y = 11;
    config->point_per_proc_z = 11;
}