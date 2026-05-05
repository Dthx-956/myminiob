#include "common/type/date_type.h"
#include "common/value.h"
#include "common/log/log.h"
#include "common/lang/sstream.h"
#include "common/lang/comparator.h"
#include "storage/common/column.h"
#include "common/type/boolean_type.h"

static bool check_date(int y, int m, int d)
{
  static int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  bool leap = (y % 400 == 0 || (y % 100 && y % 4 == 0));
  return y > 0 && (m > 0) && (m <= 12) && (d > 0) && (d <= (((m == 2 && leap) ? 1 : 0) + mon[m]));
}

int DateType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::DATES, "left type is not date");
  if (right.attr_type() == AttrType::DATES || right.attr_type() == AttrType::INTS) {
    return common::compare_int((void *)&left.value_.int_value_, (void *)&right.value_.int_value_);
  }
  return INT32_MAX;
}

int DateType::compare(const Column &left, const Column &right, int left_idx, int right_idx) const
{
  ASSERT(left.attr_type() == AttrType::DATES, "left type is not date");
  ASSERT(right.attr_type() == AttrType::DATES, "right type is not date");
  return common::compare_int((void *)&((int *)left.data())[left_idx],
                             (void *)&((int *)right.data())[right_idx]);
}

RC DateType::cast_to(const Value &val, AttrType type, Value &result) const
{
  switch (type) {
    case AttrType::CHARS: {
      string str;
      RC rc = to_string(val, str);
      if (OB_SUCC(rc)) {
        result.set_string(str.c_str());
      }
      return rc;
    }
    case AttrType::INTS: {
      result.set_int(val.get_int());
      return RC::SUCCESS;
    }
    default:
      LOG_WARN("unsupported type %d", type);
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  }
}

RC DateType::set_value_from_str(Value &val, const string &data) const
{
  int y, m, d;
  if (sscanf(data.c_str(), "%d-%d-%d", &y, &m, &d) != 3) {
    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  }
  if (!check_date(y, m, d)) {
    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  }
  int dv = y * 10000 + m * 100 + d;
  val.set_date(dv);
  return RC::SUCCESS;
}

RC DateType::to_string(const Value &val, string &result) const
{
  int value = val.value_.int_value_;
  char buf[16] = {0};
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d", value / 10000, (value % 10000) / 100, value % 100);
  result = buf;
  return RC::SUCCESS;
}
