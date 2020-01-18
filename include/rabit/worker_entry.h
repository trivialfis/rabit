#include <dmlc/any.h>
#include <bits/stdint-intn.h>
#include <rabit/internal/utils.h>
#include <cstddef>
#include <map>
#include <string>
#include <thread>
#include <netinet/in.h>
#include <rabit/internal/socket.h>

namespace rabit {

class WorkerEntry {
  utils::TCPSocket worker_;
  int32_t static constexpr kMagic = 0xff99;
  int32_t world_size_ {0};
  std::string job_id_;
  std::string cmd_;
  size_t wait_accept_ { 0 };
  in_port_t port_ { 0 };

 public:
  WorkerEntry(utils::TCPSocket sock, std::string addr) {
    worker_ = sock;
    int32_t magic {0};
    worker_.RecvAll(&magic, sizeof(magic));
    utils::Assert(magic == kMagic, "[%s]: Wrong magic number.", __func__);
    worker_.SendAll(&magic, sizeof(magic));
    worker_.RecvAll(&world_size_, sizeof(world_size_));
    worker_.RecvStr(&job_id_);
    worker_.RecvStr(&cmd_);
  }

  int32_t DecideRank(std::map<int32_t, std::string> job_map);

  std::vector<utils::TCPSocket> AssignRank(int32_t rank);
};

class RabitTracker {
  utils::TCPSocket sock_;
  int32_t n_workers_;
  std::string host_ip_;
  std::thread thread_;
  int32_t port_;

  std::vector<int32_t> Neighbors(int32_t rank, int32_t n_workers) {
    rank += 1;
    std::vector<int32_t> neighbors;
    if (rank > 1) {
      neighbors.emplace_back(rank / 2 - 1);
    }
    if (rank * 2 - 1 < n_workers) {
      neighbors.emplace_back(rank * 2 - 1);
    }
    if (rank * 2 < n_workers) {
      neighbors.emplace_back(rank * 2);
    }
    return neighbors;
  }

  std::map<std::string, std::string> WorkerfEnv() {
    return {{"DMLC_TRACKER_URI", this->host_ip_},
            {"DMLC_TRACKER_PORT", std::to_string(this->port_)}};
  }

  std::pair<std::map<int32_t, std::vector<int32_t>>,
            std::map<int32_t, int32_t>>
  Tree(int32_t n_workers) {
    std::map<int32_t, std::vector<int32_t>> tree_map;
    std::map<int32_t, int32_t> parent_map;

    for (int32_t r = 0; r < n_workers; ++r) {
      tree_map[r] = this->Neighbors(r, n_workers);
      parent_map[r] = (r + 1) / 2 - 1;
    }
    return {tree_map, parent_map};
  }

 public:
  RabitTracker();
  ~RabitTracker() {
    this->sock_.Close();
  }

  void AcceptWorkers(int32_t n_workers) {
    std::vector<int32_t> shutdown;
    std::vector<int32_t> waiting_for_connections;
    std::map<std::string, int32_t> job_map;
    std::vector<int32_t> panding;

    while (shutdown.size() != n_workers) {

    }
  }
};

}  // namespace rabit
