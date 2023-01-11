#include "indexlib/partition/index_builder.h"
#include "indexlib/partition/offline_partition_writer.h"
#include "indexlib/partition/offline_partition.h"
#include "indexlib/config/index_partition_schema.h"
#include "indexlib/config/index_partition_options.h"
#include "indexlib/document/document.h"
#include "indexlib/common_define.h"
#include "indexlib/indexlib.h"
#include "indexlib/config/index_partition_schema_maker.h"

using namespace std;
using namespace autil;
IE_NAMESPACE_USE(config);
IE_NAMESPACE_USE(test);
IE_NAMESPACE_USE(index);
IE_NAMESPACE_USE(document);
IE_NAMESPACE_USE(file_system);
IE_NAMESPACE_USE(index_base);

int main(int argc, char* argv[]) {

    IndexPartitionOptions options;
    options.SetIsOnline(false);


    return 0;
}
