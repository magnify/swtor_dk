$(document).ready(function() {

  // The countdown is only meant to be running for a short period of time.
  $('#block-block-14 .content').countdown({
    until: new Date("December 22, 2011 00:00:00"),
    layout: '<div class="jquery-countdown"><div class="countdown_row">' +
      '<span class="countdown_row countdown_descr"> ' + Drupal.t('Vi tæller ned til release!') + '</span>' +
      '<span class="countdown_section first"><span class="countdown_amount">{dnn}</span><div class="days">' + Drupal.t('Days') + '</div></span>' +
      '<span class="countdown_section second"><span class="countdown_amount">{hnn}</span><div class="hours">' + Drupal.t('Hours') + '</div></span>' +
      '<span class="countdown_section third"><span class="countdown_amount">{mnn}</span><div class="minutes">' + Drupal.t('Minutes') + '</div></span>' +
      '</div>' +
      '</div>',
    onExpiry: function(){
      render_expire();
    }
  });

});
