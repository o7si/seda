// 获取 Stage 列表
function stage_list(id) {
    $.ajax({
        url: "/stage/list",
        type: "POST",
        contentType: "application/json",
        dataType: "json",
        data: JSON.stringify(),
        success: function (result) {

            // 根据返回值生成网页内容
            var $table = $('<table class="table">');
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
                var $row=$('<tr></tr>');
                $('<td>' + result['data'][iter]['name'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['short_name'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['event_queue_size'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['thread_pool_capacity'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['performeter_capacity'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['next_state_number'] + '</td>').appendTo($row);
                $('<td>' + result['data'][iter]['is_run'] + '</td>').appendTo($row);
                $row.appendTo($table);
            }
            render_feedback_window(id, "panel-success", $table.prop("outerHTML"));
        },
        error: function (result) {
            render_feedback_window(id, "panel-danger", "error");
        }
    });
}

// 获取某个 Stage 的详细信息
function stage_info(id, stage_name) {
    $.ajax({
        url: "/stage/info",
        type: "POST",
        contentType: "application/json",
        dataType: "json",
        data: JSON.stringify({
           // "stage_name": stage_name
        }),
        success: function (result) {
            console.log(result);
        },
        error: function (result) {
            console.log(result);
            render_feedback_window(id, "panel-danger", "error");
        }
    });
}