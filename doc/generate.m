% Make sure you have installed the SimpleDoc MATLAB package.
% https://github.com/RobertDamerius/SimpleDoc

% Set title and directories
title           = 'Ground Control Station (GCS)';
inputDirectory  = 'text';
outputDirectory = ['..' filesep 'bin' filesep 'documentation' filesep 'html'];

% Use a custom navigation bar layout
layoutNavBar = [
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.none);
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Introduction', 'index.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.none);
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.text, 'GRAPHICAL USER INTERFACE');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Overview', 'guioverview.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'View', 'view.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Vehicles', 'vehicles.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Vehicle Data / Vehicle Settings', 'datasettings.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Log', 'log.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Time', 'time.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Info', 'info.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.none);
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.text, 'CONFIGURATION');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Configuration File: GCS.json', 'configuration.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Network', 'configurationnetwork.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Text-To-Speech', 'configurationtts.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Database', 'configurationdatabase.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.none);
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.text, 'MISC');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Vehicle Style', 'vehiclestyle.html');
    SimpleDoc.NavEntry(SimpleDoc.NavEntryType.link, 'Custom Vehicle Mesh', 'vehiclemesh.html');
];

% Generate HTML documentation
SimpleDoc.Make(title, inputDirectory, outputDirectory, layoutNavBar, false);

