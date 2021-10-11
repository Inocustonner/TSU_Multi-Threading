#include "helpers.hpp"
#ifdef USE_OMP
#include <omp.h>
#endif

template<>
int readv<int>(const char* msg) {
    int n;
    do {
        printf("%s", msg);
    } while (scanf_s("%d", &n) != 1);
    return n;
}

template<>
unsigned int readv<unsigned int>(const char* msg) {
    int n;
    do {
        printf("%s", msg);
    } while (scanf_s("%u", &n) != 1);
    return n;
}

template<>
double readv<double>(const char* msg) {
    double n;
    do {
        printf("%s", msg);
    } while (scanf_s("%lf", &n) != 1);
    return n;
}

template<>
float readv<float>(const char* msg) {
    float n;
    do {
        printf("%s", msg);
    } while (scanf_s("%f", &n) != 1);
    return n;    
}

namespace distribute {

invls_partition partition_intervals(int intervals, int max_threads) noexcept {
    invls_partition part = {};
    part.intervals = intervals;
    if (intervals >= max_threads) {
        part.threads = max_threads;
        part.partition_size = intervals / max_threads;
        part.ext_partitions = intervals % max_threads;
    } else {
        part.threads = intervals;
        part.partition_size = 1;
    }
    return part;
}

invl_part partition_get_i(const invls_partition& partition, int tid) noexcept {
    auto ext_partition = partition.partition_size + 1;
    if (tid <= partition.ext_partitions - 1) {
        return invl_part {
            tid * ext_partition,
            ext_partition
        };
    } else {
        return invl_part {
            partition.ext_partitions * ext_partition + (tid - partition.ext_partitions) * partition.partition_size,
            partition.partition_size
        };
    }
}

#ifdef USE_OMP
namespace omp {

// invls_partition partition_intervals(int intervals) noexcept {
//     ::distribute::partition_intervals(intervals, omp_get_num_threads());
// }

invl_part partition_get_i(const invls_partition& partition) noexcept {
    return ::distribute::partition_get_i(partition, omp_get_thread_num());
}

}
#endif

}