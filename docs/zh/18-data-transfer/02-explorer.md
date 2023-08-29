---
title: 基于可视化界面的数据接入和数据迁移
---

本节讲述使用 taos Explorer 的可视化界面进行数据迁移，使用此功能需要依赖 taosd, taosAdapter, taosX, taos-explorer 等几个服务组件。关于 taosd 和 taosAdapter 的部署请参考 [系统部署](../../deployment/deploy)，[taosX](../taosX)，以及 [部署 taos-explorer](../../operation/web)

## 功能入口

点击 explorer 左侧功能列表中的 "数据写入"，可以配置不同类型的数据源，包括 TDengine Subscription, PI, OPC-UA, OPC-DA, InfluxDB, MQTT，Kafka, CSV 等，将它们的数据写入到当前正在被管理的 TDengine 集群中。

## TDengine 订阅

进入TDengine订阅任务配置页面：
1. 在连接协议栏中，配置连接协议，默认为原生连接，可配置为WS、WSS；
2. 在服务器栏中配置服务器的 IP 或域名；
3. 在端口栏中配置连接的端口号，默认值为6030；
4. 在主题栏中，配置可以配置订阅一个或多个数据库，或超级表或普通表，也可以是一个已创建的 Topic；
5. 在认证栏，可以配置访问 TDengine 的用户名密码，用户名默认值为 root，密码默认值为 taosdata；如果数据源为云服务实例，则可以选择令牌认证方式并配置实例 token；
6. 在订阅初始位置栏，可配置从最早数据（earliest）或最晚（latest）数据开始订阅，默认为 earliest；
7. 在超时栏配置超时时间，可配置为 never: 表示无超时时间，持续进行订阅，也可指定超时时间：5s, 1m 等，支持单位 ms（毫秒），s（秒），m（分钟），h（小时），d（天），M（月），y（年）。
8. 在目标数据库栏中，选择本地 TDengine 的库作为目标库，点击 submit，即可启动一个 TDengine 订阅任务。

## Pi

1. 在 PI 数据接入页面，设置 PI 服务器的名称、AF 数据库名称。
2. 在监测点集栏，可以配置选择 Point 模式监测点集合、Point 模式监测的 AF 模板、AF 模式监测的 AF 模板。
3. 在 PI 系统设置栏，可以配置 PI 系统名，默认为 PI 服务器名。
4. 在 Data Queue 栏，可以配置 PI 连接器运行参数：MaxWaitLen（数据最大缓冲条数），默认值为 1000 ,有效取值范围为 [1,10000]；UpdateInterval（PI System 取数据频率），默认值为 10000(毫秒：ms),有效取值范围为 [10,600000]；重启补偿时间（Max Backfill Range，单位：天），每次重启服务时向前补偿该天数的数据，默认为1天。
5. 在目标数据库栏，选择需要写入的 TDengine 数据库，点击 submit ，即可启动一个 PI 数据接入任务。

## OPC-UA

1. 在 OPC-UA页面，配置 OPC-server 的地址，输入格式为 127.0.0.1:6666/OPCUA/ServerPath。
2. 在认证栏，选择访问方式。可以选择匿名访问、用户名密码访问、证书访问。使用证书访问时，需配置证书文件信息、私钥文件信息、OPC-UA 安全协议和 OPC-UA 安全策略
3. 在 Data Sets 栏，配置点位信息。(可通过“选择”按钮选择正则表达式过滤点位，每次最多能过滤出10条点位)；点位配置有两种方式：1.手动输入点位信息 2.上传csv文件配置点位信息
4. 在连接配置栏，配置连接超时间隔和采集超时间隔（单位：秒），默认值为10秒。
5. 在采集配置栏，配置采集间隔（单位：秒）、点位数量、采集模式。采集模式可选择observe（轮询模式）和subscribe（订阅模式），默认值为observe。
6. 在库表配置栏，配置目标 TDengine 中存储数据的超级表、子表结构信息。
7. 在其他配置栏，配置并行度、单次采集上报批次（默认值100）、上报超时时间（单位：秒，默认值10）、是否开启debug级别日志。
8. 在目标数据库栏，选择需要写入的 TDengine 数据库，点击 submit，即可启动一个 OPC-UA 数据接入任务。

## OPC-DA

1. 在 OPC-DA页面，配置 OPC-server 的地址，输入格式为 127.0.0.1<,localhost>/Matrikon.OPC.Simulation.1。
2. 在数据点栏，配置 OPC-DA 采集点信息。(可通过“选择”按钮选择正则表达式过滤点位，每次最多能过滤出10条点位)。点位配置有两种方式：1.手动输入点位信息 2.上传csv文件配置点位信息
3. 在连接栏，配置连接超时时间（单位：秒，默认值为10秒）、采集超时时间（单位：秒，默认值为10秒）。
4. 在库表配置栏，配置目标 TDengine 中存储数据的超级表、子表结构信息。
5. 在其他配置栏，配置并行度、单次采集上报批次（默认值100）、上报超时时间（单位：秒，默认值10）、是否开启debug级别日志。
6. 在目标数据库栏，选择需要写入的 TDengine 数据库，点击 submit，即可启动一个 OPC-DA 数据接入任务。

## InfluxDB

进入 InfluxDB 数据源同步任务的编辑页面后：
1. 在服务器地址输入框, 输入 InfluxDB 服务器的地址，可以输入 IP 地址或域名，此项为必填字段；
2. 在端口输入框, 输入 InfluxDB 服务器端口，默认情况下，InfluxDB 监听8086端口的 HTTP 请求和8088端口的 HTTPS 请求，此项为必填字段；
3. 在组织 ID 输入框，输入将要同步的组织 ID，此项为必填字段;
4. 在令牌 Token 输入框，输入一个至少拥有读取这个组织 ID 下的指定 Bucket 权限的 Token, 此项为必填字段;
5. 在同步设置的起始时间项下，通过点选选择一个同步数据的起始时间，起始时间使用 UTC 时间， 此项为必填字段;
6. 在同步设置的结束时间项下，当不指定结束时间时，将持续进行最新数据的同步；当指定结束时间时，将只同步到这个结束时间为止; 结束时间使用 UTC 时间，此项为可选字段；
7. 在桶 Bucket 输入框，输入一个需要同步的 Bucket，目前只支持同步一个 Bucket 至 TDengine 数据库，此项为必填字段；
8. 在目标数据库下拉列表，选择一个将要写入的 TDengine 目标数据库 （注意：目前只支持同步到精度为纳秒的 TDengine 目标数据库），此项为必填字段；
9. 填写完成以上信息后，点击提交按钮，即可直接启动从 InfluxDB 到 TDengine 的数据同步。

## MQTT

进入 MQTT 数据源同步任务的编辑页面后：
1. 在 MQTT 地址卡片，输入 MQTT 地址，必填字段，包括 IP 和 端口号，例如：192.168.1.10:1883;
2. 在认证卡片，输入 MQTT 连接器访问 MQTT 服务器时的用户名和密码，这两个字段为选填字段，如果未输入，即采用匿名认证的方式；
3. 在 SSL 证书卡片，可以选择是否打开 SSL/TLS 开关，如果打开此开关，MQTT 连接器和 MQTT 服务器之间的通信将采用 SSL/TLS 的方式进行加密；打开这个开关后，会出现 CA, 客户端证书和客户端私钥三个必填配置项，可以在这里输入证书和私钥文件的内容；
4. 在连接卡片，可以配置以下信息：
    - MQTT 协议：支持3.1/3.1.1/5.0三个版本；
    - Client ID: MQTT 连接器连接 MQTT 服务器时所使用的客户端 ID, 用于标识客户端的身份；
    - Keep Alive: 用于配置 MQTT 连接器与 MQTT 服务器之间的Keep Alive时间，默认值为60秒；
    - Clean Session: 用于配置 MQTT 连接器是否以Clean Session的方式连接至 MQTT 服务器，默认值为True;
    - 订阅主题及 QoS 配置：这里用来配置监听的 MQTT 主题，以及该主题支持的最大QoS, 主题和 QoS 的配置之间用::分隔，多个主题之间用,分隔，主题的配置可以支持 MQTT 协议的通配符#和+;
5. 在其他卡片，可以配置 MQTT 连接器的日志级别，支持 error, warn, info, debug, trace 5个级别，默认值为 info;
6. MQTT Payload 解析卡片，用于配置如何解析 MQTT 消息：
    - 配置表的第一行为 ts 字段，该字段为 TIMESTAMP 类型，它的值为 MQTT 连接器收到 MQTT 消息的时间；
    - 配置表的第二行为 topic 字段，为该消息的主题名称，可以选择将该字段作为列或者标签同步至 TDengine;
    - 配置表的第三行为 qos 字段，为该消息的 QoS 属性，可以选择将该字段作为列或者标签同步至 TDengine;
    - 剩余的配置项皆为自定义字段，每个字段都需要配置：字段（来源），列（目标），列类型（目标）。字段（来源）是指该 MQTT 消息中的字段名称，当前仅支持 JSON 类型的 MQTT 消息同步，可以使用 JSON Path 语法从 MQTT 消息中提取字段，例如：$.data.id; 列（目标）是指同步至 TDengine 后的字段名称；列类型（目标）是指同步至 TDengine 后的字段类型，可以从下拉列表中选择；当且仅当以上3个配置都填写后，才能新增下一个字段；
    - 如果 MQTT 消息中包含时间戳，可以选择新增一个自定义字段，将其作为同步至 TDengine 时的主键；需要注意的是，MQTT 消息中时间戳的仅支持 Unix Timestamp格式，且该字段的列类型（目标）的选择，需要与创建 TDengine 数据库时的配置一致；
    - 子表命名规则：用于配置子表名称，采用“前缀+{列类型(目标)}”的格式，例如：d{id};
    - 超级表名：用于配置同步至 TDengine 时，采用的超级表名；
7. 在目标数据库卡片，可以选择同步至 TDengine 的数据库名称，支持直接从下拉列表中选择。
8. 填写完成以上信息后，点击提交按钮，即可直接启动从 MQTT 到 TDengine 的数据同步。

## Kafka

1. 在Kafka页面，配置Kafka选项，必填字段，包括：bootstrap_server，例如192.168.1.92:9092；
2. 如果使用SSL认证，在SSL认证卡中，选择cert和cert_key的文件路径；
3. 配置其他参数，topics、topic_partitions这2个参数至少填写一个，其他参数有默认值；
4. 如果消费的Kafka数据是JSON格式，可以配置parser卡片，对数据进行解析转换；
5. 在目标数据库卡片中，选择同步到TDengine的数据库名称，支持从下拉列表中选择；
6. 填写完以上信息后，点击提交按钮，即可启动从Kafka到TDengine的数据同步。

## CSV

1. 在CSV页面，配置CSV选项，可设置忽略前N行，可输入具体的数字
2. CSV的写入配置，设置批次写入量，默认是1000
3. CSV文件解析，用于获取CSV对应的列信息：
      - 上传CSV文件或者输入CSV文件的地址
      - 选择是否包包含Header
      - 包含Header情况下直接执行下一步，查询出对应CSV的列信息，获取CSV的配置信息
      - 不包含Header情况，需要输入自定列信息，并以逗号分隔，然后下一步，获取CSV的配置信息
      - CSV的配置项，每个字段都需要配置：CSV列，DB列，列类型（目标），主键(整个配置只能有一个主键，且主键必须是TIMESTAMP类型)，作为列，作为Tag。CSV列是指该 CSV文件中的列或者自定义的列；DB列是对应的数据表的列
      - 子表命名规则：用于配置子表名称，采用“前缀+{列类型(目标)}”的格式，例如：d{id};
      - 超级表名：用于配置同步至 TDengine 时，采用的超级表名；
4. 在目标数据库卡片，可以选择同步至 TDengine 的数据库名称，支持直接从下拉列表中选择。
5. 填写完成以上信息后，点击提交按钮，即可直接启动从 CSV到 TDengine 的数据同步。


## 备份和恢复

您可以将当前连接的 TDengine 集群中的数据备份至一个或多个本地文件中，稍后可以通过这些文件进行数据恢复。本章节将介绍数据备份和恢复的具体步骤。

### 备份数据到本地文件

1. 进入系统管理页面，点击【备份】进入数据备份页面，点击右上角【新增备份】。
2. 在数据备份配置页面中可以配置三个参数：
  - 备份周期：必填项，配置每次执行数据备份的时间间隔，可通过下拉框选择每天、每 7 天、每 30 天执行一次数据备份，配置后，会在对应的备份周期的0:00时启动一次数据备份任务；
  - 数据库：必填项，配置需要备份的数据库名（数据库的 wal_retention_period 参数需大于0）；
  - 目录：必填项，配置将数据备份到 taosX 所在运行环境中指定的路径下，如 /root/data_backup；
3. 点击【确定】，可创建数据备份任务。

### 从本地文件恢复

1. 完成数据备份任务创建后，在页面中对应的数据备份任务右侧点击【数据恢复】，可将已经备份到指定路径下的数据恢复到当前 TDengine 中。