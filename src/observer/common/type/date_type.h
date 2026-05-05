#pragma once

#include "common/type/data_type.h"

class DateType : public DataType
{
public:
  DateType() : DataType(AttrType::DATES) {}
  virtual ~DateType() {}

  int compare(const Value &left, const Value &right) const override;
  int compare(const Column &left, const Column &right, int left_idx, int right_idx) const override;

  RC cast_to(const Value &val, AttrType type, Value &result) const override;

  int cast_cost(const AttrType type) override
  {
    if (type == AttrType::DATES) {
      return 0;
    } else if (type == AttrType::INTS) {
      return 1;
    } else if (type == AttrType::CHARS) {
      return 2;
    }
    return INT32_MAX;
  }

  RC set_value_from_str(Value &val, const string &data) const override;
  RC to_string(const Value &val, string &result) const override;
};
