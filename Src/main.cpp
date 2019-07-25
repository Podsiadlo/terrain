#include <iostream>
#include "Utils/Utils.h"
#include "Model/Map.h"
#include "Readers/SrtmReader.h"
#include "Split/Processor.h"

static char *const USAGE = "OPTIONS:\n"
                           "\t-i <number_of_processors_in_x_direction>\n"
                           "\t-j <number_of_processors_in_y_direction>\n"
                           "\t-k <number_of_processors_in_z_direction>\n"
                           "\t-x <number_of_points_per_processor_in_x_direction\n"
                           "\t-y <number_of_points_per_processor_in_y_direction\n"
                           "\t-z <number_of_points_per_processor_in_z_direction\n"
                           "\t-d <data_directory>\n"
                           "\t-N <north_border>\n"
                           "\t-S <south_border>\n"
                           "\t-W <west_border>\n"
                           "\t-E <east_border>\n"
                           "\t-h <top_border_in_metres_AMSL>\n"
                           "\n";

void set_default_config(Utils::config *config);

void parse_arguments(int argc, char **argv, Utils::config *config);

void
run_processor(const Utils::config *config, Map *map, double proc_width, double proc_length, double proc_height,
              int i, int j, int k, bool rotate, const char *file_base_name);

int main(int argc, char **argv) {
    //argument parsing
    Utils::config *config = static_cast<Utils::config *>(malloc(sizeof(Utils::config)));
    set_default_config(config);
    parse_arguments(argc, argv, config);
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
                run_processor(config, &map, proc_width, proc_length, proc_height, i, j, k, false, "yang");
                run_processor(config, &map, proc_width, proc_length, proc_height, i, j, k, true, "yin");
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
    config->west_border = 30.7;
    config->north_border = 50.2;
    config->east_border = 30.9;
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

void parse_arguments(int argc, char **argv, Utils::config *config) {
    int argument;
    if (argc == 1) {
        fprintf(stderr, USAGE);
    }
    while ((argument = getopt(argc, argv, "i:j:k:x:y:z:N:S:W:E:d:h:o:")) != -1)
        switch (argument) {
            case 'i':
                config->procs_x = atoi(optarg);
                break;
            case 'j':
                config->procs_y = atoi(optarg);
                break;
            case 'k':
                config->procs_z = atoi(optarg);
                break;
            case 'x':
                config->point_per_proc_x = atoi(optarg);
                break;
            case 'y':
                config->point_per_proc_x = atoi(optarg);
                break;
            case 'z':
                config->point_per_proc_x = atoi(optarg);
                break;
            case 'N':
                config->north_border = atof(optarg);
                break;
            case 'S':
                config->south_border = atof(optarg);
                break;
            case 'W':
                config->west_border = atof(optarg);
                break;
            case 'E':
                config->east_border = atof(optarg);
                break;
            case 'd':
                config->map_dir = optarg;
                break;
            case 'h':
                config->height = atoi(optarg);
                break;
            case 'o':
                config->output_filename = optarg;
                break;


//            case 't':
//                config->tolerance = atof(optarg);
//                break;
//            case 's': {
//                double s = atof(optarg);
//                config->requested_size = (size_t) s * VALUES_IN_DEGREE;
//            }
//                break;
//            case 'i':
//                config->input_filename = optarg;
//                config->read_from_ASC = true;
//                break;
//            case 'g':
//                config->post_utm = false;
//                config->pre_utm = false;
//                break;
//            case 'u':
//                config->pre_utm = true;
//                config->post_utm = false;
//                break;
//            case 'U':
//                config->pre_utm = false;
//                config->post_utm = true;
//                break;
//            case 'h':
//                config->use_height = true;
//                break;
//            case 'f':
//                config->use_floater = true;
//                break;
//            case 'm':
//                config->use_smesh = true;
//                break;
//            case 'p':
//                config->use_inp = true;
//                break;
            case '?':
                if (optopt == 'i' || optopt == 'j' || optopt == 'k' || optopt == 'o' || optopt == 'd' ||
                    optopt == 'N' || optopt == 'S' || optopt == 'W' || optopt == 'E' || optopt == 'x' ||
                    optopt == 'y' || optopt == 'z' || optopt == 'h') {

                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                    fprintf(stderr, USAGE);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                    fprintf(stderr, USAGE);
                } else {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                    fprintf(stderr, USAGE);
                }
                exit(1);
            default:
                abort();
        }
}