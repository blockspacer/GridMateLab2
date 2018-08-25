
#pragma once

#include <AzCore/std/string/string.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/JSON/document.h>
#include "CloudGemMetric/MetricsAggregator.h"
#include "CloudGemMetric/MetricsQueue.h"

/*
MetricsPriority defines priority when local file size exceeds certain threshold, which metrics to drop and which gets kept on local file
*/
namespace CloudGemMetric
{   
    namespace ServiceAPI
    {
        struct Priority;
    }

    class MetricsPriority
    {    
    public:
        MetricsPriority();
        ~MetricsPriority();       

        void InitFromBackend(const AZStd::vector<CloudGemMetric::ServiceAPI::Priority>& priorities);

        void SetEventPriority(const char* eventName, int priority);

        // metrics will be moved with move assignment to out vector after filter
        void FilterByPriority(AZStd::vector<MetricsAggregator>& metrics, int maxSizeInBytes, AZStd::vector<MetricsAggregator>& out) const;

        void SerializeToJson(rapidjson::Document& doc) const;

        bool ReadFromJson(const rapidjson::Document& doc);

    private:
        static int CompareMetrics(const AZStd::pair<const MetricsAggregator*, int>& p1, const AZStd::pair<const MetricsAggregator*, int>& p2);

    private:        
        AZStd::unordered_map<AZStd::string, int> m_eventNameToPriorityMap;
    };    
} // namespace CloudGemMetric
