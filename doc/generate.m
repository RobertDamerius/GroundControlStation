% Make sure you have installed the SimpleDoc MATLAB package.
% https://github.com/RobertDamerius/SimpleDoc

% Set title and directories
title           = 'Ground Control Station (GCS)';
inputDirectory  = 'text';
outputDirectory = ['..' filesep 'bin' filesep 'documentation' filesep 'html'];

% Use a custom navigation bar layout
layoutNavBar = SimpleDoc.NavEntry.empty();
i = 0;
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.none);
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Introduction', 'index.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.none);
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.text, 'GRAPHICAL USER INTERFACE');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Overview', 'guioverview.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'View', 'view.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Vehicles', 'vehicles.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Vehicle Data / Vehicle Settings', 'datasettings.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Log', 'log.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Time', 'time.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Info', 'info.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.none);
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.text, 'CONFIGURATION');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Configuration Files', 'configuration.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Origin', 'configurationorigin.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Network', 'configurationnetwork.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Style', 'configurationstyle.html');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.none);
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.text, 'MISC');
i = i + 1; layoutNavBar(i) = SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Custom Vehicle Mesh', 'vehiclemesh.html');

% Generate HTML documentation
SimpleDoc.Make(title, inputDirectory, outputDirectory, layoutNavBar);

