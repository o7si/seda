// 获取 Stage 列表
function stage_list(id) {
    $.ajax({
        url: '/stage/list',
        type: 'POST',
        contentType: 'application/json',
        dataType: 'json',
        data: JSON.stringify(),
        success: function (result) {

            if (result['error_code'] != 0) {
                render_feedback_window(id, 'panel-danger', result['error_desc']);
                return;
            }

            // 根据返回值生成网页内容
            var $table = $('<table class="table table-bordered table-hover"></table>');
            // 表格头部
            var $head = $('<tr></tr>');
            $('<th>' + '名称' + '</th>').appendTo($head);
            $('<th>' + '短名称' + '</th>').appendTo($head);
            $('<th>' + '事件队列长度' + '</th>').appendTo($head);
            $('<th>' + '线程池容量' + '</th>').appendTo($head);
            $('<th>' + '性能监控器容量' + '</th>').appendTo($head);
            $('<th>' + '可能的后续状态数目' + '</th>').appendTo($head);
            $('<th>' + '是否运行' + '</th>').appendTo($head);
            $head.appendTo($table);
            // 表格内容
            for (var iter in result['data']) {
                var $row = $('<tr></tr>');
                $('<td>' + result['data'][iter]['name'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['short_name'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['event_queue_size'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['thread_pool_capacity'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['performeter_capacity'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['next_state_number'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['is_run'] + '</td>').appendTo($row);
                $row.appendTo($table);
            }
            render_feedback_window(id, 'panel-success', $table.prop('outerHTML'));
        },
        error: function (result) {
            render_feedback_window(id, 'panel-danger', 'error');
        }
    });
}

// 获取某个 Stage 的详细信息
function stage_info(id, stage_name) {
    $.ajax({
        url: '/stage/info',
        type: 'POST',
        contentType: 'application/json',
        dataType: 'json',
        data: JSON.stringify({
            'stage_name': stage_name
        }),
        success: function (result) {

            console.log(result);
            if (result['error_code'] != 0) {
                render_feedback_window(id, 'panel-danger', result['error_desc']);
                return;
            }

            // 根据返回值生成网页内容
            var $table = $('<table class="table table-bordered table-hover"></table>');
            // 表格内容
            var $name = $('<tr></tr>');
            $('<td style="width: 30%;">' + '名称' + '</td>').appendTo($name);
            $('<td>' + result['data']['name'] + '</td>').appendTo($name);
            $name.appendTo($table);

            var $short_name = $('<tr></tr>');
            $('<td>' + '短名称' + '</td>').appendTo($short_name);
            $('<td>' + result['data']['short_name'] + '</td>').appendTo($short_name);
            $short_name.appendTo($table);

            var $event_queue_size = $('<tr></tr>');
            $('<td>' + '事件队列的长度' + '</td>').appendTo($event_queue_size);
            $('<td>' + result['data']['event_queue_size'] + '</td>').appendTo($event_queue_size);
            $event_queue_size.appendTo($table);

            var $thread_pool_name = $('<tr></tr>');
            $('<td>' + '线程池的名称' + '</td>').appendTo($thread_pool_name);
            $('<td>' + result['data']['thread_pool_name'] + '</td>').appendTo($thread_pool_name);
            $thread_pool_name.appendTo($table);

            var $thread_pool_capacity = $('<tr></tr>');
            $('<td>' + '线程池的容量' + '</td>').appendTo($thread_pool_capacity);
            $('<td>' + result['data']['thread_pool_capacity'] + '</td>').appendTo($thread_pool_capacity);
            $thread_pool_capacity.appendTo($table);

            var $performeter_name = $('<tr></tr>');
            $('<td>' + '性能监控器的名称' + '</td>').appendTo($performeter_name);
            $('<td>' + result['data']['performeter_name'] + '</td>').appendTo($performeter_name);
            $performeter_name.appendTo($table);

            var $performeter_capacity = $('<tr></tr>');
            $('<td>' + '性能监控器的容量' + '</td>').appendTo($performeter_capacity);
            $('<td>' + result['data']['performeter_capacity'] + '</td>').appendTo($performeter_capacity);
            $performeter_capacity.appendTo($table);

            var $next_state_number = $('<tr></tr>');
            $('<td>' + '可能的后续状态数量' + '</td>').appendTo($next_state_number);
            $('<td>' + result['data']['next_state_number'] + '</td>').appendTo($next_state_number);
            $next_state_number.appendTo($table);

            var $next_state_info = $('<tr></tr>');
            $('<td>' + '可能的后续状态信息' + '</td>').appendTo($next_state_info);
            $('<td style="word-break: break-all;">' + result['data']['next_state_info'] + '</td>').appendTo($next_state_info);
            $next_state_info.appendTo($table);

            var $is_run = $('<tr></tr>');
            $('<td>' + '是否运行在系统中' + '</td>').appendTo($is_run);
            $('<td>' + result['data']['is_run'] + '</td>').appendTo($is_run);
            $is_run.appendTo($table);

            var $longest_dura = $('<tr></tr>');
            $('<td>' + '最长耗时（线程等待 + 任务执行）' + '</td>').appendTo($longest_dura);
            $('<td>' + result['data']['longest_dura'] + '</td>').appendTo($longest_dura);
            $longest_dura.appendTo($table);

            var $longest_wait_dura = $('<tr></tr>');
            $('<td>' + '最长耗时（线程等待）' + '</td>').appendTo($longest_wait_dura);
            $('<td>' + result['data']['longest_wait_dura'] + '</td>').appendTo($longest_wait_dura);
            $longest_wait_dura.appendTo($table);

            var $longest_exec_dura = $('<tr></tr>');
            $('<td>' + '最长耗时（任务执行）' + '</td>').appendTo($longest_exec_dura);
            $('<td>' + result['data']['longest_exec_dura'] + '</td>').appendTo($longest_exec_dura);
            $longest_exec_dura.appendTo($table);

            var $sum_dura = $('<tr></tr>');
            $('<td>' + '总耗时（线程等待 + 任务执行）' + '</td>').appendTo($sum_dura);
            $('<td>' + result['data']['sum_dura'] + '</td>').appendTo($sum_dura);
            $sum_dura.appendTo($table);

            var $sum_wait_dura = $('<tr></tr>');
            $('<td>' + '总耗时（线程等待）' + '</td>').appendTo($sum_wait_dura);
            $('<td>' + result['data']['sum_wait_dura'] + '</td>').appendTo($sum_wait_dura);
            $sum_wait_dura.appendTo($table);

            var $sum_exec_dura = $('<tr></tr>');
            $('<td>' + '总耗时（任务执行）' + '</td>').appendTo($sum_exec_dura);
            $('<td>' + result['data']['sum_exec_dura'] + '</td>').appendTo($sum_exec_dura);
            $sum_exec_dura.appendTo($table);

            var $avg_dura = $('<tr></tr>');
            $('<td>' + '平均耗时（线程等待 + 任务执行）' + '</td>').appendTo($avg_dura);
            $('<td>' + result['data']['avg_dura'] + '</td>').appendTo($avg_dura);
            $avg_dura.appendTo($table);

            var $avg_wait_dura = $('<tr></tr>');
            $('<td>' + '平均耗时（线程等待）' + '</td>').appendTo($avg_wait_dura);
            $('<td>' + result['data']['avg_wait_dura'] + '</td>').appendTo($avg_wait_dura);
            $avg_wait_dura.appendTo($table);

            var $avg_exec_dura = $('<tr></tr>');
            $('<td>' + '平均耗时（任务执行）' + '</td>').appendTo($avg_exec_dura);
            $('<td>' + result['data']['avg_exec_dura'] + '</td>').appendTo($avg_exec_dura);
            $avg_exec_dura.appendTo($table);

            var $lastest_dura_list = $('<tr></tr>');
            $('<td>' + '最新的耗时列表（线程等待 + 任务执行）' + '</td>').appendTo($lastest_dura_list);
            $('<td style="word-break: break-all;">' + result['data']['lastest_dura_list'].slice(-20) + '</td>').appendTo($lastest_dura_list);
            $lastest_dura_list.appendTo($table);

            var $lastest_wait_dura_list = $('<tr></tr>');
            $('<td>' + '最新的耗时列表（线程等待）' + '</td>').appendTo($lastest_wait_dura_list);
            $('<td style="word-break: break-all;">' + result['data']['lastest_wait_dura_list'].slice(-20) + '</td>').appendTo($lastest_wait_dura_list);
            $lastest_wait_dura_list.appendTo($table);

            var $lastest_exec_dura_list = $('<tr></tr>');
            $('<td>' + '最新的耗时列表（任务执行）' + '</td>').appendTo($lastest_exec_dura_list);
            $('<td style="word-break: break-all;">' + result['data']['lastest_exec_dura_list'].slice(-20) + '</td>').appendTo($lastest_exec_dura_list);
            $lastest_exec_dura_list.appendTo($table);

            var $task_count = $('<tr></tr>');
            $('<td>' + '总执行次数' + '</td>').appendTo($task_count);
            $('<td>' + result['data']['task_count'] + '</td>').appendTo($task_count);
            $task_count.appendTo($table);

            render_feedback_window(id, 'panel-success', $table.prop('outerHTML'));
        },
        error: function (result) {
            render_feedback_window(id, 'panel-danger', 'error');
        }
    });
}

// 修改 Stage 的某个字段
function stage_update(id, stage_name, key, value) {
    $.ajax({
        url: '/stage/update',
        type: 'POST',
        contentType: 'application/json',
        dataType: 'json',
        data: JSON.stringify({
            'stage_name': stage_name,
            'key': key,
            'value': value
        }),
        success: function (result) {

            if (result['error_code'] != 0) {
                render_feedback_window(id, 'panel-danger', result['error_desc']);
                return;
            }

            render_feedback_window(id, 'panel-success', result['data']);
        },
        error: function (result) {
            render_feedback_window(id, 'panel-danger', 'error');
        }
    });
}

// 绘制 Stage 的耗时图表
function stage_dura_chart(id, stage_name) {

    // 生成一个 Canvas 用于绘图
    // canvas 的外部容器
    var $container = $('<div style="position: relative; margin: auto; width:80vw; height: 80vh;"></div>')
    var chart_id = "chart_" + id.slice(9);
    $('<canvas id="' + chart_id + '"></canvas>').appendTo($container);

    // 添加到页面
    render_feedback_window(id, 'panel-primary', $container.prop('outerHTML'));
    var ctx = document.getElementById(chart_id).getContext('2d');

    function draw_chart(id, stage_name, ctx) {
        $.ajax({
            url: '/stage/info',
            type: 'POST',
            contentType: 'application/json',
            dataType: 'json',
            data: JSON.stringify({
                'stage_name': stage_name
            }),
            success: function (result) {

                if (result['error_code'] != 0) {
                    render_feedback_window(id, 'panel-danger', result['error_desc']);
                    return;
                }

                // 图表显示的数据数量为 50 个
                var number = 50;

                var label = [];
                for (var i = 0; i < number; ++i)
                    label[i] = '';

                var dura_list = result['data']['lastest_dura_list'];
                dura_list = dura_list.slice(-50);
                for (var i = 0; i < dura_list.length; ++i)
                    dura_list[i] *= 1000;
                var dura_wait_list = result['data']['lastest_wait_dura_list'];
                dura_wait_list = dura_wait_list.slice(-50);
                for (var i = 0; i < dura_wait_list.length; ++i)
                    dura_wait_list[i] *= 1000;
                var dura_exec_list = result['data']['lastest_exec_dura_list'];
                dura_exec_list = dura_exec_list.slice(-50);
                for (var i = 0; i < dura_exec_list.length; ++i)
                    dura_exec_list[i] *= 1000;

                new Chart(ctx, {
                    type: 'line',
                    data: {
                        labels: label,
                        datasets: [
                            {
                                label: "线程等待 + 任务执行",
                                fill: false,
                                pointBackgroundColor: "rgba(250, 177, 160, 1.0)",
                                pointBorderWidth: 0,
                                borderColor: "rgba(250, 177, 160, 1.0)",
                                borderWidth: 2,
                                data: dura_list
                            },
                            {
                                label: "线程等待",
                                fill: false,
                                pointBackgroundColor: "rgba(255, 234, 167, 1.0)",
                                pointBorderWidth: 0,
                                borderColor: "rgba(255, 234, 167, 1.0)",
                                borderWidth: 2,
                                data: dura_wait_list
                            },
                            {
                                label: "任务执行",
                                fill: false,
                                pointBackgroundColor: "rgba(178, 190, 195, 1.0)",
                                pointBorderWidth: 0,
                                borderColor: "rgba(178, 190, 195, 1.0)",
                                borderWidth: 2,
                                data: dura_exec_list
                            }
                        ]
                    },
                    options: {
                        animation: false,
                        scales: {
                            xAxes: [{
                                gridLines: {
                                    display: false
                                }
                            }],
                            yAxes: [{
                                gridLines: {
                                    display: true
                                }
                            }]
                        }
                    }
                });
                render_feedback_window_only_scene(id, 'panel-success');
            },
            error: function (result) {
                render_feedback_window_only_scene(id, 'panel-danger');
            }
        });
    }

    setInterval(() => {
        draw_chart(id, stage_name, ctx);
    }, 1000);
}