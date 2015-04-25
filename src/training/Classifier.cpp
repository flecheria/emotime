/**
 *
 * @file    Classifier.cpp
 *
 * @brief   The Classifier implementation
 *
 * @details
 *
 */

#include "Classifier.h"
#include "matrix_io.h"

#include <fstream>
#include <iostream>

using std::ifstream;
using std::cerr;
using std::cout;
using std::endl;

using std::string;
using std::vector;
using std::pair;

using cv::Mat;

namespace emotime {

  Classifier::Classifier() {

  }

  Classifier::~Classifier() {

  }

  bool Classifier::train(std::string csv_file) {
    // create a vector of couple <string, int> where
    // to load the csv
    vector<pair<string, int> > files_classes;
    // DEBUG
    cout << "vector files_classes is created" << endl;
    // if loading data fails stop the process
    if ( !this->loadTrainData(csv_file, files_classes) ) {
      return false;
    }
    // if the csv files are present but they're empty
    if (files_classes.size() == 0){
      cerr << "ERR: empty samples!" << endl;
      return false;
    }
    // DEBUG
    row_n = 0
    cout << "vector row 0 = " << files_classes.at(row_n).first << " " << files_classes.at(row_n).second  << endl;
    // create a OpenCv matrix where to store the result of the gabor filter
    Mat sample = matrix_io_load(files_classes.at(0).first);
    
    // calculate the total number of feature inside the
    // loaded gabor filter
    int nfeatures = sample.rows * sample.cols;
    // create two new matrix
    Mat train_data = Mat(0, nfeatures, CV_32FC1);
    Mat train_labels = Mat(0, 1, CV_32FC1);

    for(size_t i = 0; i < files_classes.size(); i++) {
      string fpath = files_classes.at(i).first;

      sample = matrix_io_load(fpath);
      Mat sample_float;

      if (sample.type() != CV_32FC1) {
        sample.convertTo(sample_float, CV_32FC1);
      } else {
        sample_float=sample;
      }
      train_data.push_back(sample_float.reshape(1, 1)); // reshape(int channels, int rows)
      train_labels.push_back((float)files_classes.at(i).second);
    }

    return this->doTraining(train_data, train_labels);
  }


  bool Classifier::loadTrainData(string csv_file,
      std::vector<std::pair<std::string, int> >& sample_files_classes) {
    try {
      // create the csv streaming
      ifstream csvf(csv_file.c_str());

      string sampleF;
      string sampleC;
      int sclass;

      while (getline(csvf, sampleC, ',')) {
        // What is N and P is a mistery
        getline(csvf, sampleF, '\n');

        // TODO maybe:
        // N = negative result of the gabor filter
        // P = positive result of the gabor filter
        if (sampleC == "N") {
          sclass = 0;
        } else {
          sclass = 1;
        }
        // add a pair to the vector with:
        // <address to the gabor file, the classification value>
        sample_files_classes.push_back(std::make_pair(sampleF, sclass));
      }

      return true;

    } catch (int e) {
      cerr << "ERR: Something wrong during csv loading #" << e << endl;
      return false;
    }
  }
}
