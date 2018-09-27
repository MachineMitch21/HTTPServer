
"use strict";

$(document).ready(function() {
    $("#housePicIndicators").carousel({
        interval: false
    });

    $("#housePicIndicators").on('slid.bs.carousel', function() {
        console.log("slid event");
    });
});