(function ($) {
    $.fn.formAjax2 = function (options) {
        var _self = this;
        // This is the easiest way to have default options.
        var settings = $.extend({
            validator: function (e) {},
            beforeLoadData: function (e) {},
            afterLoadData: function (e) {}
        }, options);

        _self.getFormData = function () {
            var a = _self.serializeArray();
            var r = {};
            for (var i in a) {
                r[a[i]['name']] = a[i]['value'];
            }
            // Непроставленные чекбоксы тоже отправляем
            _self.find("input:checkbox:not(:checked)").each(function (index) {
                var _ = $(this);
                if (_.is(':disabled')) {
                    return;
                }
                r[_.attr('name')] = false;
            });
            return r;
        };
        _self.disable = function () {
            _self.find('input, textarea, button, select').attr('disabled', 'disabled');
        };
        _self.enable = function () {
            _self.find('input, textarea, button, select').removeAttr('disabled');
        }
        _self.on('submit', function (e) {
            e.preventDefault();
            var url = _self.attr('action');
            var data = _self.getFormData();
            var event = {
                submit: e,
                form: _self,
                data: data,
                success: true
            };
            settings.validator(event);
            if (!event.success) {
                return;
            }
            _self.disable();
            $.ajax({
                type: "POST",
                url: url,
                data: JSON.stringify(data),
                contentType: 'application/json;charset=utf-8',
                dataType: 'json',
                complete: function () {
                    _self.enable();
                    setTimeout(function () {
                        _self.trigger('reset');
                    }, 2000);
                },
                success: function (data) {}
            });
        });
        _self.on('reset', function (e) {
            e.preventDefault();
            var url = _self.attr('action');
            _self.disable();
            $.ajax({
                type: "GET",
                url: url,
                success: function (data) {
                    settings.beforeLoadData(data);
                    for (var name in data) {
                        var inputObject = _self.find('[name="' + name + '"]');
                        if (!inputObject) {
                            continue;
                        }
                        var valObject = _self.find('#' + name + '_value');
                        var type = valObject ? inputObject.attr('type') : undefined;
                        var tagName = valObject ? inputObject.prop('tagName') : undefined;

                        if (tagName === 'SELECT') {
                            inputObject.val(data[name]);
                            if (valObject) {
                                valObject.html(inputObject.find('option[value="' + data[name] + '"]').text());
                            }

                        } else if (type === 'checkbox') {
                            inputObject.prop('checked', !!data[name]);
                            if (valObject) {
                                valObject.html(data[name] ? 'Yes' : 'No');
                            }
                        } else {
                            inputObject.val(data[name]);
                            if (valObject) {
                                valObject.html(data[name]);
                            }
                        }
                    }
                    settings.afterLoadData(data);
                },
                complete: function () {
                    _self.enable();
                }
            });
        });
        _self.trigger('reset');
        return _self;
    };
}(jQuery));