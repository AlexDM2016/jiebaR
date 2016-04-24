require(["gitbook", "jquery"], function(gitbook, $) {

    gitbook.events.bind("page.change", function() {
        if (!$('.toolbar-delete').length)
            gitbook.toolbar.createButton({
                position: 'left',
                icon: 'fa fa-history',
                className: 'toolbar-delete', //,
                onClick: function() {
                    translatePage();
                }
            });
        translateInitilization();
    })

});
