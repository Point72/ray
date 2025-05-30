// Copyright 2021 The Ray Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "absl/container/flat_hash_map.h"
#include "ray/common/bundle_location_index.h"
#include "ray/common/bundle_spec.h"
#include "ray/common/id.h"
#include "ray/common/placement_group.h"

namespace ray {
namespace raylet_scheduling_policy {

// Options that controls the scheduling behavior.
struct SchedulingContext {
  virtual ~SchedulingContext() = default;
};

struct BundleSchedulingContext : public SchedulingContext {
 public:
  explicit BundleSchedulingContext(
      std::optional<std::shared_ptr<BundleLocations>> bundle_locations)
      : bundle_locations_(std::move(bundle_locations)) {}

  /// The locations of existing bundles for this placement group.
  std::optional<std::shared_ptr<BundleLocations>> bundle_locations_;
};

struct AffinityWithBundleSchedulingContext : public SchedulingContext {
 public:
  explicit AffinityWithBundleSchedulingContext(const BundleID &bundle_id)
      : affinity_bundle_id_(bundle_id) {}
  const BundleID &GetAffinityBundleID() const { return affinity_bundle_id_; }

 private:
  BundleID affinity_bundle_id_;
};

struct NodeLabelSchedulingContext : public SchedulingContext {
 public:
  explicit NodeLabelSchedulingContext(const rpc::SchedulingStrategy &scheduling_strategy)
      : scheduling_strategy_(scheduling_strategy) {}
  const rpc::SchedulingStrategy &GetSchedulingStrategy() const {
    return scheduling_strategy_;
  }

 private:
  rpc::SchedulingStrategy scheduling_strategy_;
};

}  // namespace raylet_scheduling_policy
}  // namespace ray
