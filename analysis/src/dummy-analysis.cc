#include <cstdint>
#include <delphes_tools/genparticle.h>
#include <delphes_tools/weight.h>
#include <delphes_tools/utils.h>

#include <argparse/argparse.hpp>

#include <TFile.h>
#include <TTree.h>

#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;


bool isGoodJet(const Jet* jet, const float jet_pt_min,
               const float jet_abs_eta_max) {
  return (jet->PT > jet_pt_min) and (std::abs(jet->Eta) < jet_abs_eta_max);
}

std::vector<const Jet*> runJetSelection(const TClonesArray* jet_arr,
                                  const float jet_pt_min,
                                  const float jet_abs_eta_max) {
  std::vector<const Jet*> output{};

  for (int32_t jet_idx = 0; jet_idx < jet_arr->GetEntries(); ++jet_idx) {
    if (auto jet = dynamic_cast<const Jet*>(jet_arr->At(jet_idx))) {
      if (isGoodJet(jet, jet_pt_min, jet_abs_eta_max)) {
        output.push_back(jet);
      }

    } else {
      std::cerr << "got a wrong jet" << std::endl;

    }
  }
  return output;
}





int main(int argc, char *argv[]) {
  gErrorIgnoreLevel = 3000;

  argparse::ArgumentParser parser{"parser_name"};
  parser.add_argument("-i", "--input-path").required();
  parser.add_argument("-o", "--output-path").required();
  try {
    parser.parse_args(argc, argv);

  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << parser;
    return 1;

  }

  const fs::path input_path = parser.get<std::string>("input-path");

  std::cout << "input_path: " << input_path << std::endl;

  setupDelphes();

  TFile* input_file = TFile::Open(input_path.c_str());
  TTree* input_tree = dynamic_cast<TTree*>(input_file->Get("Delphes"));

  #define SET_BRANCH_ADDRESS(branch, var) \
      TClonesArray* var = nullptr; \
      input_tree->SetBranchAddress(branch, &var);

  SET_BRANCH_ADDRESS("Particle", input_gen_particle_arr);
  SET_BRANCH_ADDRESS("Jet", input_jet_arr);

  const int64_t input_num_entries = input_tree->GetEntries();
  for (int64_t event_idx = 0; event_idx < input_num_entries; event_idx++) {
    input_tree->GetEntry(event_idx);

    std::cout << "event: " << event_idx << std::endl;

    const auto jet_vec = runJetSelection(input_jet_arr, /*jet_pt_min=*/36.0, /*jet_abs_eta_max=*/2.4);
    for (const auto jet : jet_vec) {
      std::cout << "jet.pt: " << jet->PT << std::endl;
    }
  }

}
