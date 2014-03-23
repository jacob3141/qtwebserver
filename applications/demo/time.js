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

var document = response.document();
var head = document.elementsByTagName('head')[0];
var body = document.elementsByTagName('body')[0];

head
.createElementUnder('title')
.createTextUnder('The title');

body
.createElementUnder('p')
.createTextUnder('This is a paragraph.');

body
.createElementUnder('p')
.createTextUnder('This is another paragraph. ')
.createTextAfter('And this text has been appended to the previous node.');					

true;

