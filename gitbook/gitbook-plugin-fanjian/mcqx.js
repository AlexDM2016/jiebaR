require(["gitbook", "jquery"], function(gitbook, $) {

//    var init = function() {
//        if(!$('.toolbar-delete').length)
//            gitbook.toolbar.createButton({
//                position: 'left',
//                icon: 'fa fa-history',
//                className: 'toolbar-delete',//,
//                onClick: function() {
//                  translatePage();
//                }
////
////                    $(this).prepend('<p class="historyMessage">History cleared. Refreshing ...</p>');
////                    $('.historyMessage').hide().fadeIn('slow').delay(2000).fadeOut('slow').delay(1).queue(function() {
////                        $(this).remove();
////                        location.reload();
//                   }
//                                        );
//                
//            };
    gitbook.events.bind("page.change",function() {
        if(!$('.toolbar-delete').length)
            gitbook.toolbar.createButton({
                position: 'left',
                icon: 'fa fa-history',
                className: 'toolbar-delete',//,
                onClick: function() {
                  translatePage();
                }
//
//                    $(this).prepend('<p class="historyMessage">History cleared. Refreshing ...</p>');
//                    $('.historyMessage').hide().fadeIn('slow').delay(2000).fadeOut('slow').delay(1).queue(function() {
//                        $(this).remove();
//                        location.reload();
                   }
                                        );
      translateInitilization();      
            })

    //gitbook.events.bind("page.change", translateInitilization);
});
