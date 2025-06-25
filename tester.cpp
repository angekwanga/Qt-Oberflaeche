#include <cstddef>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <gtest/gtest.h>
#include "types.h"
#include "scheduled_trip.h"
#include "network.h"

using namespace bht;

namespace {

inline unsigned int convertTime(const GTFSTime t) {
  return t.hour * 3600 + t.minute * 60 + t.second;
}

inline bool containsStop(const std::vector<Stop>& items, const std::string &needle) {
  for (auto item : items) {
    if (item.id == needle) {
      return true;
    }
  }

  return false;
}

// Tests for getStopsForTransfer
TEST(Network, getStopsForTransfer) {
  std::string inputDirectory{"/GTFSTest"};
  Network network{inputDirectory};

  // S Potsdam Hauptbahnhof - Bahnsteig Gleis 4
  std::string stopId = "de:12054:900230999:2:53";
  std::vector<Stop> results = network.getStopsForTransfer(stopId);
  std::vector<std::string> desiredStops{ "de:12054:900230999", "de:12054:900230999::1", "de:12054:900230999::2", "de:12054:900230999::3", "de:12054:900230999::4", "de:12054:900230999::5", "de:12054:900230999::6", "de:12054:900230999::7", "de:12054:900230999:1:50", "de:12054:900230999:1:51", "de:12054:900230999:2:52", "de:12054:900230999:2:53" };
  EXPECT_EQ(results.size(), desiredStops.size()) << "transfers at " << stopId << " should contain " << desiredStops.size() << " stops but your size() is " << results.size();
  for (const std::string& stopTestId : desiredStops) {
    EXPECT_TRUE(containsStop(results, stopTestId)) << "Stop " << stopTestId << " not in stops for transfer at " << stopId << " but should be";
  }

  // S Potsdam Hauptbahnhof
  stopId = "de:12054:900230999";
  results = network.getStopsForTransfer(stopId);
  for (const std::string& stopTestId : desiredStops) {
    EXPECT_TRUE(containsStop(results, stopTestId)) << "Stop " << stopTestId << " not in stops for transfer at " << stopId << " but should be";
  }

  // Biesenthal, Bahnhof
  stopId = "de:12060:900350137:2:51";
  results = network.getStopsForTransfer(stopId);
  desiredStops = std::vector<std::string>{"de:12060:900350137", "de:12060:900350137:2:51", "de:12060:900350137:1:50"};
  for (const std::string& stopTestId : desiredStops) {
    EXPECT_TRUE(containsStop(results, stopTestId)) << "Stop " << stopTestId << " not in stops for transfer at " << stopId << " but should be";
  }
}

// Tests for getTravelPlanDepartingAt
TEST(Network, getTravelPlanDepartingAt) {
  std::string inputDirectory{"/GTFSTest"};
  Network network{inputDirectory};

  GTFSTime d{.hour = 12, .minute = 0, .second = 0};
  std::vector<StopTime> results = network.getTravelPlanDepartingAt("de:12054:900230999", "de:13071:12353", d);
  EXPECT_GT(results.size(), 2) << "Path from 'de:12054:900230999' to 'de:13071:12353' should be available but your size() is " << results.size();

  // Check if first stop is the a associated stop
  StopTime first = results[0];
  auto startStops = network.getStopsForTransfer("de:12054:900230999");
  std::vector<Stop>::iterator match = startStops.begin();
  while (match != startStops.end()) {
    if (match->id == first.stopId) {
      break;
    }
    ++match;
  }
  EXPECT_FALSE(match == startStops.end()) << "First stop in your path is not at the start station";
  
  StopTime last = results[results.size()-1];
  auto destinationStops = network.getStopsForTransfer("de:13071:12353");
  match = destinationStops.begin();
  while (match != destinationStops.end()) {
    if (match->id == last.stopId) {
      break;
    }
    ++match;
  }
  EXPECT_FALSE(match == destinationStops.end()) << "Final stop in your path is not at the destination station";

  for (int index = 1; index < results.size(); index++) {
    StopTime s = results[index-1];
    StopTime d = results[index];

    EXPECT_LE(convertTime(s.departureTime), convertTime(d.departureTime)) << "Departure time of stop " << s.stopId << " should be ahead of departure time of stop " << d.stopId;
  }
}

} // namespace
