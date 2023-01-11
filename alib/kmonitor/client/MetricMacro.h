/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Created on: 2020-06-17 17:12
 * Author Name: dongdong.shan
 * Author Email: dongdong.shan@alibaba-inc.com
 */

#pragma once

// register base

#define REGISTER_MUTABLE_METRIC_BASE(target, name, metricType, statisticsType, level) \
    do {                                                                \
        std::string metricName = (name);                                \
        target = manager->declareMutableMetrics(metricName, (metricType), (statisticsType), (level)); \
        if (nullptr == target) {                                        \
            AUTIL_LOG(ERROR, "failed to register metric:[%s]", metricName.c_str()); \
        }                                                               \
    } while (0)

#define REGISTER_STATUS_MUTABLE_METRIC_BASE(target, name, level)           \
    REGISTER_MUTABLE_METRIC_BASE(target, name, kmonitor::STATUS,           \
                                 0, level)

#define REGISTER_QPS_MUTABLE_METRIC_BASE(target, name, level)           \
    REGISTER_MUTABLE_METRIC_BASE(target, name, kmonitor::QPS,           \
                                 0, level)

#define REGISTER_GAUGE_MUTABLE_METRIC_BASE(target, name, level)         \
    REGISTER_MUTABLE_METRIC_BASE(target, name, kmonitor::GAUGE,         \
                                 kmonitor::StatisticsType::SUM | kmonitor::StatisticsType::COUNT | \
                                 kmonitor::StatisticsType::MIN | kmonitor::StatisticsType::MAX, level)

#define REGISTER_SUMMARY_GAUGE_MUTABLE_METRIC_BASE(target, name, level) \
    REGISTER_MUTABLE_METRIC_BASE(target, name, kmonitor::GAUGE,         \
                                 kmonitor::StatisticsType::SUM | kmonitor::StatisticsType::COUNT | \
                                 kmonitor::StatisticsType::MIN | kmonitor::StatisticsType::MAX | \
                                 kmonitor::StatisticsType::SUMMARY | kmonitor::StatisticsType::PERCENTILE_99 | \
                                 kmonitor::StatisticsType::PERCENTILE_95, \
                                 level)

#define REGISTER_GAUGE_MUTABLE_METRIC(target, name)                     \
    REGISTER_GAUGE_MUTABLE_METRIC_BASE(target, name, kmonitor::NORMAL)

#define REGISTER_LENGTH_MUTABLE_METRIC(target, name)                    \
    REGISTER_MUTABLE_METRIC_BASE(target, name, kmonitor::GAUGE,         \
                                 kmonitor::StatisticsType::SUM | kmonitor::StatisticsType::COUNT, kmonitor::NORMAL)

#define REGISTER_LATENCY_MUTABLE_METRIC(target, name)                   \
    REGISTER_SUMMARY_GAUGE_MUTABLE_METRIC_BASE(target, name, kmonitor::NORMAL)

#define REGISTER_STATUS_MUTABLE_METRIC(target, name)                    \
    REGISTER_STATUS_MUTABLE_METRIC_BASE(target, name, kmonitor::NORMAL)


#define REGISTER_QPS_MUTABLE_METRIC(target, name)                       \
    REGISTER_QPS_MUTABLE_METRIC_BASE(target, name, kmonitor::NORMAL)

#define REPORT_MUTABLE_METRIC(metric, value) \
    if (metric) {                            \
        metric->Report(tags, value);         \
    }

#define REPORT_MUTABLE_QPS(metric)              \
    if (metric) {                               \
        metric->Report(tags, 1);                \
    }

#define REPORT_USER_MUTABLE_QPS_TAGS(reporter, name, pTags)     \
    if ((reporter)) {                                           \
        (reporter)->report(1, name, kmonitor::QPS, pTags);      \
    }

#define REPORT_USER_MUTABLE_METRIC_TAGS(reporter, name, value, pTags)   \
    if ((reporter)) {                                                   \
        (reporter)->report(value, name, kmonitor::GAUGE, pTags);        \
    }

#define REPORT_USER_MUTABLE_LATENCY_TAGS(reporter, name, value, pTags)  \
    if ((reporter)) {                                                   \
        (reporter)->report(value, name, kmonitor::GAUGE, pTags, true);  \
    }

#define REPORT_USER_MUTABLE_QPS(reporter, name)                 \
    REPORT_USER_MUTABLE_QPS_TAGS(reporter, name, nullptr)

#define REPORT_USER_MUTABLE_METRIC(reporter, name, value)               \
    REPORT_USER_MUTABLE_METRIC_TAGS(reporter, name, value, nullptr)

#define REPORT_USER_MUTABLE_LATENCY(reporter, name, value)              \
    REPORT_USER_MUTABLE_LATENCY_TAGS(reporter, name, value, nullptr)
