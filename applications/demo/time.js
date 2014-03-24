//
// Copyright 2010-2014 Jacob Dawid <jacob.dawid@cybercatalyst.net>
//
// This file is part of Shark.
//
// Shark is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shark is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shark.  If not, see <http://www.gnu.org/licenses/>.
//

// @serverbegin
function server_main(request, response) {
	var document = response.document();
	var head = document.elementsByTagName('head')[0];
	var body = document.elementsByTagName('body')[0];

	head
	.createElementBelow('title')
	.createTextBelow('The title');

	body
	.createElementBelow('p')
	.createTextBelow('Responding to request');

	body
	.createElementBelow('p')
	.createTextBelow(request.requestString());

	body
	.createElementBelow('div').withClass('time')
	.createTextBelow('This is the time.');
}
// @serverend

// @clientbegin
function client_main() {
	function startTime() {
		var today = new Date();
		var h = padZeros(today.getHours());
		var m = padZeros(today.getMinutes());
		var s = padZeros(today.getSeconds());
	
		document.getElementsByClassName('time')[0].innerHTML = h + ":" + m + ":" + s;
		
		setTimeout(startTime, 1000);
	}

	function padZeros(i) {
		return i < 10 ? ("0" + i) : i;
	}

	startTime();
}
// @clientend
