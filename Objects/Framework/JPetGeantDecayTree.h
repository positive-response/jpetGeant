/**
 *  @copyright Copyright 2021 The J-PET Monte Carlo Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @file JPetGeantDecayTree.h
 */

#ifndef JPETGEANTDECAYTREE_H
#define JPETGEANTDECAYTREE_H 1

#include <TObject.h>
#include <vector>
#include <map>

/**
 * @class JPetGeantDecayTree
 * @brief Class stores decay tree structures (in form of nodes and tracks)
 */

enum InteractionType
{
  kPrimaryGamma,
  kScattActivePart,
  kScattNonActivePart,
  kSecondaryPart,
  kUnknownInteractionType
};

enum DecayChannel { 
  kPara2G, kDirect2G, kOrtho2G, kPara3G, kDirect3G, kOrtho3G, kUnknown
};

struct Branch {
  Branch() {};
  Branch(int trackID, int primaryBranch);
  int fTrackID = -1;             //ID of the track corresponding to this branch
  int fPrimaryBranchID = -1;       //-1 for branch coming from primary photon, primary branchId otherwise
  std::vector<int> fNodeIDs = {};    //container for all of the nodes
  std::vector<InteractionType> fInteractionType = {};
  
  void AddNodeID(int nodeID, InteractionType interactionType);
  int GetTrackID() const { return fTrackID; };
  int GetPrimaryBranchID() const { return fPrimaryBranchID; };
  int GetNumberOfNodes() { return fNodeIDs.size(); };
  int GetPrimaryNodeID() const { if (fNodeIDs.size()) return fNodeIDs[0]; else return -1; };
  int GetLastNodeID() const { if (fNodeIDs.size()) return fNodeIDs[fNodeIDs.size()-1]; else return -1; };
  int GetPreviousNodeID(int nodeID) const;
  InteractionType GetInteractionType(int nodeID) const;
};

class JPetGeantDecayTree : public TObject
{

public:
  JPetGeantDecayTree();
  ~JPetGeantDecayTree();
  
  void Clean();
  void ClearVectors();
  void CopyDecayTree(JPetGeantDecayTree* decayTree);
  
  void SetEventNumber(int eventID) { fEventID = eventID; };
  void SetDecayChannel(DecayChannel decayChannel) { fDecayChannel = decayChannel; };
  int FindPrimaryPhoton(int nodeID);
  void AddNodeToBranch(int nodeID, int trackID, InteractionType interactionType);
  void AddBranch(Branch branch) {fBranches.push_back(branch); };
  void SetTrackBranchConnections(std::map<int, int> trackBranchConnections) {fTrackBranchConnection = trackBranchConnections; };
  int GetEventNumber() { return fEventID; };
  Branch GetBranch(unsigned trackID) const;
  int GetNumberOfBranches() { return fBranches.size(); };
  DecayChannel GetDecayChannel() { return fDecayChannel; };
  std::map<int, int> GetTrackBranchConnections() {return fTrackBranchConnection; };

private:
  int fEventID = 0;
  DecayChannel fDecayChannel = DecayChannel::kUnknown;
  std::map<int, int> fTrackBranchConnection = {};
  std::vector<Branch> fBranches = {};
     
  ClassDef(JPetGeantDecayTree,3)

};

#endif /* !JPETGEANTDECAYTREE_H */
