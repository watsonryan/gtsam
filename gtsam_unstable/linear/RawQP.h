#pragma once

#include <gtsam_unstable/linear/QP.h>
#include <gtsam/base/Matrix.h>
#include <gtsam/inference/Key.h>

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <gtsam/inference/Symbol.h>


namespace gtsam {
class RawQP {
  typedef std::vector<std::pair<Key, Matrix> > coefficient_v;

  std::vector<std::pair<Key, Matrix> > g;
  std::unordered_map<std::string, std::unordered_map<std::string, coefficient_v > > row_to_map;
  std::unordered_map<std::string, Key> varname_to_key;
  std::unordered_map<std::string, coefficient_v > IL;
  std::unordered_map<std::string, coefficient_v > IG;
  std::unordered_map<std::string, coefficient_v > E;
  std::unordered_map<std::string, double> b;
  std::unordered_map<std::string, std::unordered_map<std::string, double> > H;
  unsigned int varNumber;
  double f;
  std::string obj_name;
  std::string name_;
  const bool debug = true;
public:
  RawQP() : g(), row_to_map(), varname_to_key(), IL(), IG(), E(), b(), H(), varNumber(1){
  }

  void setName(std::vector<char> name) {
    name_ = std::string(name.begin(), name.end());
    if (debug) {
      std::cout << "Parsing file: " << name_ << std::endl;
    }
  }

  void addColumn(std::vector<char> var, std::vector<char> row,
      double coefficient) {
    std::string var_(var.begin(), var.end()), row_(row.begin(), row.end());

    if(!varname_to_key.count(var_))
      varname_to_key.insert({var_, Symbol('X', varNumber++)});

    if(row_ == obj_name){
      g.push_back(std::make_pair(varname_to_key[var_], coefficient * ones(1,1)));
      return;
    }
    row_to_map[row_][row_].push_back({varname_to_key[var_], coefficient * ones(1,1)});

    if (debug) {
      std::cout << "Added Column for Var: " << var_ << " Row: " << row_
          << " Coefficient: " << coefficient << std::endl;
    }
  }

  void addColumn(std::vector<char> var, std::vector<char> row1,
      double coefficient1, std::vector<char> row2, double coefficient2) {
    std::string var_(var.begin(), var.end()), row1_(row1.begin(), row1.end()),
        row2_(row2.begin(), row2.end());
    if(!varname_to_key.count(var_))
      varname_to_key.insert({var_, Symbol('X', varNumber++)});

    if(row1_ == obj_name)
      row_to_map[row1_][row2_].push_back({varname_to_key[var_], coefficient1 * ones(1,1)});
    else
      row_to_map[row1_][row1_].push_back({varname_to_key[var_], coefficient1 * ones(1,1)});

    if(row2_ == obj_name)
      row_to_map[row2_][row2_].push_back({varname_to_key[var_], coefficient2 * ones(1,1)});
    else
      row_to_map[row2_][row2_].push_back({varname_to_key[var_], coefficient2 * ones(1,1)});

    if (debug) {
      std::cout << "Added Column for Var: " << var_ << " Row: " << row1_
          << " Coefficient: " << coefficient1 << std::endl;
      std::cout << "                      " << "Row: " << row2_
          << " Coefficient: " << coefficient2 << std::endl;
    }
  }

  void addRHS(std::vector<char> var, std::vector<char> row,
      double coefficient) {
    std::string var_(var.begin(), var.end()), row_(row.begin(), row.end());
    if(row_ == obj_name)
      f = -coefficient;
    else
      b.insert({row_, coefficient});

    if (debug) {
      std::cout << "Added RHS for Var: " << var_ << " Row: " << row_
          << " Coefficient: " << coefficient << std::endl;
    }
  }

  void addRHS(std::vector<char> var, std::vector<char> row1,
      double coefficient1, std::vector<char> row2, double coefficient2) {
    std::string var_(var.begin(), var.end()), row1_(row1.begin(), row1.end()),
        row2_(row2.begin(), row2.end());

    if(row1_ == obj_name)
      f = -coefficient1;
    else
      b.insert({row1_, coefficient1});

    if(row2_ == obj_name)
      f = -coefficient2;
    else
      b.insert({row2_, coefficient2});

    if (debug) {
      std::cout << "Added RHS for Var: " << var_ << " Row: " << row1_
          << " Coefficient: " << coefficient1 << std::endl;
      std::cout << "                      " << "Row: " << row2_
          << " Coefficient: " << coefficient2 << std::endl;
    }
  }

  void addRow(char type, std::vector<char> name) {
    std::string name_(name.begin(), name.end());
    switch(type){
      case 'N':
        obj_name = name_;
        break;
      case 'L':
        row_to_map.insert({name_, IL});
        IL.insert({name_, coefficient_v()});
        break;
      case 'G':
        row_to_map.insert({name_, IG});
        IG.insert({name_, coefficient_v()});
        break;
      case 'E':
        row_to_map.insert({name_, E});
        E.insert({name_, coefficient_v()});
        break;
      default:
        std::cout << "invalid type: " << type << std::endl;
        break;
    }
    if (debug) {
      std::cout << "Added Row Type: " << type << " Name: " << name_
          << std::endl;
    }
  }

  void addBound(std::vector<char> type, std::vector<char> var, double number) {
    std::string type_(type.begin(), type.end()), var_(var.begin(), var.end());


    if (debug) {
      std::cout << "Added Bound Type: " << type_ << " Var: " << var_
          << " Amount: " << number << std::endl;
    }
  }

  void addBound(std::vector<char> type, std::vector<char> var) {
    std::string type_(type.begin(), type.end()), var_(var.begin(), var.end());
    if (debug) {
      std::cout << "Added Free Bound Type: " << type_ << " Var: " << var_
          << " Amount: " << std::endl;
    }
  }

  void addQuadTerm(std::vector<char> var1, std::vector<char> var2,
      double coefficient) {
    std::string var1_(var1.begin(), var1.end()), var2_(var2.begin(),
        var2.end());
    if (debug) {
      std::cout << "Added QuadTerm for Var: " << var1_ << " Row: " << var2_
          << " Coefficient: " << coefficient << std::endl;
    }
  }

  QP makeQP() {
    return QP();
  }
};
}