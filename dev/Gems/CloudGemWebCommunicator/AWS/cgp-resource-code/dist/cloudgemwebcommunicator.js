!function(a){function b(a,b,c){a in i||(i[a]={name:a,declarative:!0,deps:b,declare:c,normalizedDeps:b})}function c(a){return m[a]||(m[a]={name:a,dependencies:[],exports:{},importers:[]})}function d(b){if(!b.module){var e=b.module=c(b.name),f=b.module.exports,g=b.declare.call(a,function(a,b){if(e.locked=!0,"object"==typeof a)for(var c in a)f[c]=a[c];else f[a]=b;for(var d=0,g=e.importers.length;g>d;d++){var h=e.importers[d];if(!h.locked)for(var i=0;i<h.dependencies.length;++i)h.dependencies[i]===e&&h.setters[i](f)}return e.locked=!1,b},b.name);e.setters=g.setters,e.execute=g.execute;for(var j=0,k=b.normalizedDeps.length;k>j;j++){var l,n=b.normalizedDeps[j],o=i[n],p=m[n];p?l=p.exports:o&&!o.declarative?l=o.esModule:o?(d(o),p=o.module,l=p.exports):l=h(n),p&&p.importers?(p.importers.push(e),e.dependencies.push(p)):e.dependencies.push(null),e.setters[j]&&e.setters[j](l)}}}function e(b){var c={};if(("object"==typeof b||"function"==typeof b)&&b!==a)if(k)for(var d in b)"default"!==d&&f(c,b,d);else{var e=b&&b.hasOwnProperty;for(var d in b)"default"===d||e&&!b.hasOwnProperty(d)||(c[d]=b[d])}return c.default=b,l(c,"__useDefault",{value:!0}),c}function f(a,b,c){try{var d;(d=Object.getOwnPropertyDescriptor(b,c))&&l(a,c,d)}catch(d){return a[c]=b[c],!1}}function g(b,c){var d=i[b];if(d&&!d.evaluated&&d.declarative){c.push(b);for(var e=0,f=d.normalizedDeps.length;f>e;e++){var k=d.normalizedDeps[e];-1==j.call(c,k)&&(i[k]?g(k,c):h(k))}d.evaluated||(d.evaluated=!0,d.module.execute.call(a))}}function h(a){if(o[a])return o[a];if("@node/"==a.substr(0,6))return o[a]=e(n(a.substr(6)));var b=i[a];if(!b)throw"Module "+a+" not present.";return d(i[a]),g(a,[]),i[a]=void 0,b.declarative&&l(b.module.exports,"__esModule",{value:!0}),o[a]=b.declarative?b.module.exports:b.esModule}var i={},j=Array.prototype.indexOf||function(a){for(var b=0,c=this.length;c>b;b++)if(this[b]===a)return b;return-1},k=!0;try{Object.getOwnPropertyDescriptor({a:0},"a")}catch(a){k=!1}var l;!function(){try{Object.defineProperty({},"a",{})&&(l=Object.defineProperty)}catch(a){l=function(a,b,c){try{a[b]=c.value||c.get.call(a)}catch(a){}}}}();var m={},n="undefined"!=typeof System&&System._nodeRequire||"undefined"!=typeof require&&require.resolve&&"undefined"!=typeof process&&require,o={"@empty":{}};return function(a,c,d,f){return function(g){g(function(g){for(var i=0;i<c.length;i++)!function(a,b){b&&b.__esModule?o[a]=b:o[a]=e(b)}(c[i],arguments[i]);f({register:b});var j=h(a[0]);if(a.length>1)for(var i=1;i<a.length;i++)h(a[i]);return d?j.default:j})}}}("undefined"!=typeof self?self:global)(["4f"],["3","8","6","7","9","a","d","44","1c","54","10","11"],!1,function(a){this.require,this.exports,this.module;a.register("50",["3"],function(a,b){"use strict";var c,d,e=this&&this.__extends||function(){var a=Object.setPrototypeOf||{__proto__:[]}instanceof Array&&function(a,b){a.__proto__=b}||function(a,b){for(var c in b)b.hasOwnProperty(c)&&(a[c]=b[c])};return function(b,c){function d(){this.constructor=b}a(b,c),b.prototype=null===c?Object.create(c):(d.prototype=c.prototype,new d)}}();b&&b.id;return{setters:[function(a){c=a}],execute:function(){d=function(a){function b(b,c,d,e,f){return a.call(this,b,c,d,e,f)||this}return e(b,a),b.prototype.listAllChannels=function(){return a.prototype.get.call(this,"portal/channel")},b.prototype.listAllUsers=function(){return a.prototype.get.call(this,"portal/users")},b.prototype.setUserStatus=function(b){return a.prototype.post.call(this,"portal/users",b)},b.prototype.register=function(b){return a.prototype.get.call(this,"portal/registration/"+b)},b}(c.ApiHandler),a("WebCommunicatorApi",d)}}}),a.register("51",["8","6","52","7","9","a"],function(a,b){"use strict";var c,d,e,f,g,h,i,j=this&&this.__decorate||function(a,b,c,d){var e,f=arguments.length,g=f<3?b:null===d?d=Object.getOwnPropertyDescriptor(b,c):d;if("object"==typeof Reflect&&"function"==typeof Reflect.decorate)g=Reflect.decorate(a,b,c,d);else for(var h=a.length-1;h>=0;h--)(e=a[h])&&(g=(f<3?e(g):f>3?e(b,c,g):e(b,c))||g);return f>3&&g&&Object.defineProperty(b,c,g),g},k=this&&this.__metadata||function(a,b){if("object"==typeof Reflect&&"function"==typeof Reflect.metadata)return Reflect.metadata(a,b)};b&&b.id;return{setters:[function(a){c=a},function(a){d=a},function(a){e=a},function(a){f=a},function(a){g=a},function(a){h=a}],execute:function(){i=function(){function a(a,b,d){this.http=a,this.aws=b,this.metricservice=d,this.displayName="Web Communicator",this.srcIcon="https://m.media-amazon.com/images/G/01/cloudcanvas/images/Speech_Recognition_Gem_optimized._V518452893_.png",this.state=new c.TackableStatus,this.metric=new c.TackableMeasure}return a.prototype.ngOnInit=function(){this._apiHandler=new e.WebCommunicatorApi(this.context.ServiceUrl,this.http,this.aws,this.metricservice,this.context.identifier),this.report(this.metric),this.assign(this.state)},a.prototype.report=function(a){var b=this;a.name="Channels",a.value="Loading...",this._apiHandler.listAllChannels().subscribe(function(c){var d=JSON.parse(c.body.text());a.value=b.getChannelNum(d.result)},function(b){a.value="Offline"})},a.prototype.assign=function(a){a.label="Loading",a.styleType="Loading",this._apiHandler.get("service/status").subscribe(function(b){var c=JSON.parse(b.body.text());a.label="online"==c.result.status?"Online":"Offline",a.styleType="online"==c.result.status?"Enabled":"Offline"},function(b){a.label="Offline",a.styleType="Offline"})},a.prototype.getChannelNum=function(a){for(var b=0,c=[],d=0,e=a;d<e.length;d++){var f=e[d];-1==c.indexOf(f.ChannelName)&&(c.push(f.ChannelName),b++)}return b},j([d.Input(),k("design:type",Object)],a.prototype,"context",void 0),j([d.Input(),k("design:type",String)],a.prototype,"displayName",void 0),j([d.Input(),k("design:type",String)],a.prototype,"srcIcon",void 0),a=j([d.Component({selector:"cloudgemwebcommunicator-thumbnail",template:'\n    <thumbnail-gem \n        [title]="displayName" \n        [cost]="\'Low\'" \n        [srcIcon]="srcIcon" \n        [metric]="metric" \n        [state]="state" \n        >\n    </thumbnail-gem>'}),k("design:paramtypes",[f.Http,g.AwsService,h.LyMetricService])],a)}(),a("WebCommunicatorThumbnailComponent",i)}}}),a.register("53",["7","6","d","9","52","8","44","1c","54","a"],function(a,b){"use strict";var c,d,e,f,g,h,i,j,k,l,m,n,o=this&&this.__extends||function(){var a=Object.setPrototypeOf||{__proto__:[]}instanceof Array&&function(a,b){a.__proto__=b}||function(a,b){for(var c in b)b.hasOwnProperty(c)&&(a[c]=b[c])};return function(b,c){function d(){this.constructor=b}a(b,c),b.prototype=null===c?Object.create(c):(d.prototype=c.prototype,new d)}}(),p=this&&this.__decorate||function(a,b,c,d){var e,f=arguments.length,g=f<3?b:null===d?d=Object.getOwnPropertyDescriptor(b,c):d;if("object"==typeof Reflect&&"function"==typeof Reflect.decorate)g=Reflect.decorate(a,b,c,d);else for(var h=a.length-1;h>=0;h--)(e=a[h])&&(g=(f<3?e(g):f>3?e(b,c,g):e(b,c))||g);return f>3&&g&&Object.defineProperty(b,c,g),g},q=this&&this.__metadata||function(a,b){if("object"==typeof Reflect&&"function"==typeof Reflect.metadata)return Reflect.metadata(a,b)};b&&b.id;return{setters:[function(a){c=a},function(a){d=a},function(a){e=a},function(a){f=a},function(a){g=a},function(a){h=a},function(a){i=a},function(a){j=a},function(a){k=a},function(a){l=a}],execute:function(){!function(a){a[a.List=0]="List",a[a.BanUser=1]="BanUser",a[a.SendMessageViaChannel=2]="SendMessageViaChannel",a[a.SendMessageToUser=3]="SendMessageToUser"}(m||(m={})),a("Mode",m),n=function(a){function b(b,c,d,e,f,g){var h=a.call(this)||this;return h.http=b,h.aws=c,h.toastr=d,h.metric=f,h.breadcrumbs=g,h.subNavActiveIndex=0,h.pageSize=10,h.channels=[],h.simplifiedChannels=[],h.users=[],h.filteredUsers=[],h.usersOnCurrentPage=[],h.filterCondition="",h.listenToChannelAction=new j.ActionItem("Listen",h.initializeListeningToChannelPage),h.broadcastAction=new j.ActionItem("Send message",h.sendMessageViaChannelModal),h.channelActions=[h.listenToChannelAction,h.broadcastAction],h.listenToUserAction=new j.ActionItem("Listen",h.initializeListeningToUserPage),h.banUserAction=new j.ActionItem("Ban",h.banUserModal),h.unbanUserAction=new j.ActionItem("Unban",h.unbanUser),h.sendMessageToUserAction=new j.ActionItem("Send message",h.sendMessageToUserModal),h.registeredUserActions=[h.listenToUserAction,h.banUserAction,h.sendMessageToUserAction],h.registeredUserProfileActions=[h.banUserAction,h.sendMessageToUserAction],h.bannedUserActions=[h.unbanUserAction],h.toastr.setRootViewContainerRef(e),h}return o(b,a),b.prototype.ngOnInit=function(){this._apiHandler=new g.WebCommunicatorApi(this.context.ServiceUrl,this.http,this.aws,this.metric,this.context.identifier),this.dismissModal=this.dismissModal.bind(this),this.banUserModal=this.banUserModal.bind(this),this.listenToChannelAction.onClick=this.listenToChannelAction.onClick.bind(this),this.broadcastAction.onClick=this.broadcastAction.onClick.bind(this),this.listenToUserAction.onClick=this.listenToUserAction.onClick.bind(this),this.banUserAction.onClick=this.banUserAction.onClick.bind(this),this.unbanUserAction.onClick=this.unbanUserAction.onClick.bind(this),this.sendMessageToUserAction.onClick=this.sendMessageToUserAction.onClick.bind(this),this.webCommunicatorModes=m,this.registerCGPClient()},b.prototype.getSubNavItem=function(a){(this.isListeningToChannel||this.isListeningToUser)&&(this.breadcrumbs.removeLastBreadcrumb(),this.isListeningToChannel=!1,this.isListeningToUser=!1),this.subNavActiveIndex=a,0==a&&this.updateUsers(),1==a&&(this.mode=m.List,this.updateChannels())},b.prototype.updateUsers=function(){this.isListeningToUser=!1,this.isLoadingUsers=!0,this.getAllUsers().then(function(){this.isLoadingUsers=!1}.bind(this),function(){this.isLoadingUsers=!1}.bind(this))},b.prototype.banUser=function(a){this.setUserStatus(a,"BANNED"),this.modalRef.close()},b.prototype.createOPENSSLUser=function(){var a=this;this._apiHandler.register("OPENSSL").subscribe(function(b){var c=JSON.parse(b.body.text()),d=c.result,e=d.PrivateKey,f=d.DeviceCert,g={endpoint:d.Endpoint,endpointPort:d.EndpointPort,connectionType:d.ConnectionType};a.downloadCertificateFile(e,"webcommunicatorkey.pem","text/csv"),setTimeout(function(){return a.downloadCertificateFile(f,"webcommunicatordevice.pem","text/csv")},1e3),setTimeout(function(){return a.downloadCertificateFile(JSON.stringify(g),"deviceInfo.json","application/json")},2e3),a.updateUsers()},function(b){a.toastr.error("The user was not created successfully. "+b.message)})},b.prototype.unbanUser=function(a){this.setUserStatus(a,"REGISTERED")},b.prototype.initializeListeningToUserPage=function(a){this.currentUser=a,this.channelType="allChannels",this.channels.length>0?(this.listenToUser(this.currentUser.ClientID,this.channelType),this.isListeningToUser=!0,this.breadcrumbs.addBreadcrumb(a.ClientID,null)):this.getAllChannels().then(function(){this.listenToUser(this.currentUser.ClientID,this.channelType),this.isListeningToUser=!0,this.breadcrumbs.addBreadcrumb(a.ClientID,null)}.bind(this))},b.prototype.updateChannels=function(){this.isListeningToChannel=!1,this.isLoadingChannels=!0,this.getAllChannels().then(function(){this.isLoadingChannels=!1}.bind(this),function(){this.isLoadingChannels=!1}.bind(this))},b.prototype.initializeListeningToChannelPage=function(a){this.currentChannel=a,this.breadcrumbs.addBreadcrumb(a.ChannelName,null),this.users.length>0&&this.listenToDefaultChannel(),this.getAllUsers().then(function(){this.listenToDefaultChannel()}.bind(this))},b.prototype.sendMessageViaChannel=function(){var a=JSON.parse(JSON.stringify(this.currentChannel));a.CommunicationType="allClients"==this.selectedUser?"BROADCAST":"PRIVATE",this.publishMessage(a,this.selectedUser),this.modalRef.close()},b.prototype.sendMessageToUser=function(){for(var a=new g.ChannelEntry({}),b=0,c=this.channels;b<c.length;b++){var d=c[b];if(d.ChannelName==this.selectedChannel&&"PRIVATE"==d.CommunicationType){a=d;break}}this.publishMessage(a,this.currentUser.ClientID),this.modalRef.close()},b.prototype.sendMessageToUserModal=function(a){if(this.currentUser=a,this.newMessage="",this.selectedChannel="",this.channels.length>0)return this.setDefaultChannel(),void(this.mode=m.SendMessageToUser);this.getAllChannels().then(function(){this.setDefaultChannel(),this.mode=m.SendMessageToUser}.bind(this))},b.prototype.sendMessageViaChannelModal=function(a){if(this.currentChannel=a,this.selectedChannel=this.currentChannel.ChannelName,this.newMessage="",this.users.length>0)return this.selectedUser="PRIVATE"==a.CommunicationType?this.users[0].ClientID:"allClients",void(this.mode=m.SendMessageViaChannel);this.getAllUsers().then(function(){this.selectedUser="PRIVATE"==a.CommunicationType?this.users[0].ClientID:"allClients",this.mode=m.SendMessageViaChannel}.bind(this))},b.prototype.dismissModal=function(){this.mode=m.List},b.prototype.banUserModal=function(a){this.currentUser=a,this.mode=m.BanUser},b.prototype.sortUsersByColumn=function(a,b){this.sort(this.users,a,b),this.sort(this.filteredUsers,a,b),this.updatePaginationInfo(this.currentUserPage)},b.prototype.updatePaginationInfo=function(a){if(0==this.subNavActiveIndex){var b=this.filteredUsers.length;this.userPages=Math.ceil(b/this.pageSize);var c=(a-1)*this.pageSize,d=a*this.pageSize;this.usersOnCurrentPage=this.filteredUsers.slice(c,d),this.currentUserPage=a,this.pagination&&(this.pagination.currentPage=a)}},b.prototype.filterClientList=function(){var a=this;this.filteredUsers=this.users.filter(function(b){return 0===b.ClientID.indexOf(a.filterCondition)}),this.updatePaginationInfo(1)},b.prototype.registerCGPClient=function(){var a=this;this.isRegisteringClient=!0,this._apiHandler.register("WEBSOCKET").subscribe(function(b){var c=JSON.parse(b.body.text()),d=AWS.config.credentials;a.createMqttClient(c.result.Endpoint,d),a.isRegisteringClient=!1,a.updateUsers()},function(b){a.toastr.error("The client did not register properly. "+b.message),a.isRegisteringClient=!1})},b.prototype.createMqttClient=function(a,b){var c=this.getSignedUrl(a,this.aws.context.region,b);this.client=new k.Paho.MQTT.Client(c,this.aws.context.authentication.user.username+this.generateUuid()),this.connect()},b.prototype.connect=function(){return new Promise(function(a,b){if(this.client.isConnected())a();else{var c={onSuccess:function(){a()}.bind(this),useSSL:!0,timeout:60,mqttVersion:4,onFailure:function(){this.toastr.error("Connection failed."),b()}.bind(this)};this.client.connect(c)}}.bind(this))},b.prototype.subscribeToChannel=function(a,b){var c=this.getChannelSubscription(a,b);return new Promise(function(b,d){this.connect().then(function(){this.client.subscribe(c,{onSuccess:function(){b()}.bind(this),onFailure:function(){this.toastr.error("Failed to subscribe to the channel "+a.ChannelName+" ."),d()}.bind(this)})}.bind(this))}.bind(this))},b.prototype.generateUuid=function(){for(var a=this.generateRandonmCharacters()+this.generateRandonmCharacters(),b=0;b<4;++b)a+="-",a+=this.generateRandonmCharacters();return a+=this.generateRandonmCharacters()+this.generateRandonmCharacters()},b.prototype.generateRandonmCharacters=function(){return Math.floor(65536*(1+Math.random())).toString(16).substring(1)},b.prototype.getAllUsers=function(){return new Promise(function(a,b){var c=this;this._apiHandler.listAllUsers().subscribe(function(b){var d=JSON.parse(b.body.text());c.updateUsersInfo(d.result.UserInfoList),a()},function(a){c.toastr.error("The user list did not refresh properly. "+a.message),b()})}.bind(this))},b.prototype.updateUsersInfo=function(a){this.users=[],this.ascOrder=!0;for(var b=0,c=a;b<c.length;b++){var d=c[b];this.users.push(new g.UserEntry(d))}this.sort(this.users,"ClientID",this.ascOrder),this.filterClientList()},b.prototype.setUserStatus=function(a,b){var c=this,d={ClientID:a.ClientID,RegistrationStatus:b,CGPUser:!0};this._apiHandler.setUserStatus(d).subscribe(function(b){var c=JSON.parse(b.body.text());a.RegistrationStatus=c.result.SetUserStatusResult.RegistrationStatus},function(a){c.toastr.error("The user status did not change properly. "+a.message)})},b.prototype.downloadCertificateFile=function(a,b,c){var d=new Blob([a],{type:c});if(window.navigator.msSaveOrOpenBlob)window.navigator.msSaveBlob(d,b);else{var e=document.createElement("a"),f=URL.createObjectURL(d);e.href=f,e.download=b,window.document.body.appendChild(e),e.click(),window.document.body.removeChild(e),URL.revokeObjectURL(f)}},b.prototype.listenToUser=function(a,b){var c=this;this.messageList=[];for(var d=[],e=0,f=this.channels;e<f.length;e++){var g=f[e];if("allChannels"==b||g.ChannelName==b){var h=this.getChannelSubscription(g,a);-1==d.indexOf(h)&&d.push(this.getChannelSubscription(g,a)),this.subscribeToChannel(g,a)}}this.client.onMessageArrived=function(a){if(d.indexOf(a.destinationName)>-1){var e="(BROADCAST) ";a.destinationName.indexOf("/client/")>-1&&(e="(PRIVATE) ");var f=JSON.parse(a.payloadString);"allChannels"!=b&&f.ChannelName!=b||c.messageList.push(e+f.Message)}}},b.prototype.publishMessage=function(a,b){this.subscribeToChannel(a,b).then(function(){var c=new k.Paho.MQTT.Message(JSON.stringify({ChannelName:a.ChannelName,Message:this.newMessage}));c.destinationName=this.getChannelSubscription(a,b),this.client.send(c)}.bind(this))},b.prototype.setDefaultChannel=function(){for(var a=0,b=this.simplifiedChannels;a<b.length;a++){var c=b[a];if("BROADCAST"!=c.CommunicationType)return void(this.selectedChannel=c.ChannelName)}this.selectedChannel=""},b.prototype.getAllChannels=function(){return new Promise(function(a,b){var c=this;this._apiHandler.listAllChannels().subscribe(function(b){var d=JSON.parse(b.body.text());c.updateChannelsInfo(d.result),a()},function(a){c.toastr.error("The channel list did not refresh properly. "+a.message),b()})}.bind(this))},b.prototype.updateChannelsInfo=function(a){this.channels=[],this.simplifiedChannels=[],this.ascOrder=!0;for(var b={},c=0,d=a;c<d.length;c++){var e=d[c];this.channels.push(new g.ChannelEntry(e)),b[e.ChannelName]?b[e.ChannelName].CommunicationType="BROADCAST & PRIVATE":b[e.ChannelName]=JSON.parse(JSON.stringify(e))}for(var f=0,h=Object.keys(b);f<h.length;f++){var i=h[f];this.simplifiedChannels.push(new g.ChannelEntry(b[i]))}this.sort(this.simplifiedChannels,"ChannelName",this.ascOrder)},b.prototype.listenToDefaultChannel=function(){var a=this;this.messageList=[];var b=[];if(-1!==this.currentChannel.CommunicationType.indexOf("BROADCAST")){var c=this.subscribeToActualChannel(this.currentChannel,"BROADCAST","allClients");b.push(c)}if(-1!==this.currentChannel.CommunicationType.indexOf("PRIVATE"))for(var d=0,e=this.users;d<e.length;d++){var f=e[d],c=this.subscribeToActualChannel(this.currentChannel,"PRIVATE",f.ClientID);b.push(c)}this.client.onMessageArrived=function(c){if(-1!==b.indexOf(c.destinationName)){var d=JSON.parse(c.payloadString),e=-1===c.destinationName.indexOf("/client/")?"(BROADCAST) ":"(PRIVATE) ";d.ChannelName==a.currentChannel.ChannelName&&a.messageList.push(e+d.Message)}},this.isListeningToChannel=!0},b.prototype.subscribeToActualChannel=function(a,b,c){var d=JSON.parse(JSON.stringify(a));return d.CommunicationType=b,this.subscribeToChannel(d,c),this.getChannelSubscription(d,c)},b.prototype.getChannelSubscription=function(a,b){var c=this.getCommunicationChannel(a),d=c.Subscription;if("PRIVATE"==a.CommunicationType){d="";for(var e=c.Subscription.split("/"),f=0;f<e.length-1;f++)d+=e[f],d+="/";d+=b}return d},b.prototype.getCommunicationChannel=function(a){for(var b=0,c=this.channels;b<c.length;b++){var d=c[b],e=!a.CommunicationChannel&&a.ChannelName==d.ChannelName&&a.CommunicationType==d.CommunicationType;if(e=e||a.CommunicationChannel==d.ChannelName&&a.CommunicationType==d.CommunicationType)return d}return new g.ChannelEntry({})},b.prototype.getChannelCommunicationType=function(a){for(var b=0,c=this.simplifiedChannels;b<c.length;b++){var d=c[b];if(d.ChannelName==a)return d.CommunicationType}return""},b.prototype.sort=function(a,b,c){var d=this;this.ascOrder=c,a.sort(function(a,c){return a[b]<c[b]?d.ascOrder?-1:1:a[b]>c[b]?d.ascOrder?1:-1:0})},b.prototype.getSignedUrl=function(a,b,c){var d=AWS.util.date.iso8601(new Date).replace(/[:\-]|\.\d{3}/g,""),e=d.substr(0,8),f="GET",g="wss",h="/mqtt",i="iotdevicegateway",j="AWS4-HMAC-SHA256",k=e+"/"+b+"/"+i+"/aws4_request",l="X-Amz-Algorithm="+j;l+="&X-Amz-Credential="+encodeURIComponent(c.accessKeyId+"/"+k),l+="&X-Amz-Date="+d,l+="&X-Amz-SignedHeaders=host";var m="host:"+a+"\n",n=AWS.util.crypto.sha256("","hex"),o=f+"\n"+h+"\n"+l+"\n"+m+"\nhost\n"+n,p=j+"\n"+d+"\n"+k+"\n"+AWS.util.crypto.sha256(o,"hex"),q=this.getSignatureKey(c.secretAccessKey,e,b,i);return l+="&X-Amz-Signature="+AWS.util.crypto.hmac(q,p,"hex"),c.sessionToken&&(l+="&X-Amz-Security-Token="+encodeURIComponent(c.sessionToken)),g+"://"+a+h+"?"+l},b.prototype.getSignatureKey=function(a,b,c,d){var e=AWS.util.crypto.hmac("AWS4"+a,b,"buffer"),f=AWS.util.crypto.hmac(e,c,"buffer"),g=AWS.util.crypto.hmac(f,d,"buffer");return AWS.util.crypto.hmac(g,"aws4_request","buffer")},p([d.Input(),q("design:type",Object)],b.prototype,"context",void 0),p([d.ViewChild(e.ModalComponent),q("design:type",e.ModalComponent)],b.prototype,"modalRef",void 0),p([d.ViewChild("pagination"),q("design:type",Object)],b.prototype,"pagination",void 0),b=p([d.Component({selector:"web-communicator-index",template:'<facet-generator [context]="context"                  [tabs]="[\'Users\', \'Channel List\']"                  (tabClicked)="getSubNavItem($event)" [metricIdentifier]="context.identifier"></facet-generator> <div *ngIf="subNavActiveIndex == 0">     <div *ngIf="!isListeningToUser">         <button class="btn l-primary" (click)="createOPENSSLUser()">             Create OPENSSL User         </button>         <i class="fa fa-question-circle-o" aria-hidden="true" placement="right" ngbTooltip="Put the generated files into your Application/Platform\'s user storage under @user@/certs/aws.  For example, in the Lumberyard PC Editor this is dev\\Cache\\<Game Project>\\pc\\user\\certs\\aws"></i>         <input class="form-control col-3 float-right" type="text" [(ngModel)]="filterCondition" (ngModelChange)="filterClientList()"/>         <label class="search-label float-right">Client ID</label>     </div>     <div *ngIf="!isListeningToUser">         <div [ngSwitch]="isLoadingUsers || isRegisteringClient">             <div *ngSwitchCase="true">                 <div class="loading-spinner-container">                     <loading-spinner></loading-spinner>                 </div>             </div>             <div class="content-container" *ngSwitchCase="false">                 <div *ngIf="!users || users.length == 0">                     No user                 </div>                 <div *ngIf="users.length > 0">                     <table class="table table-hover">                         <thead>                             <tr>                                 <th class="client-id-column" (click)="sortUsersByColumn(\'ClientID\', !ascOrder)">                                     CLIENT ID                                     <i *ngIf="ascOrder" class="fa fa-sort-amount-asc" aria-hidden="true"></i>                                     <i *ngIf="!ascOrder" class="fa fa-sort-amount-desc" aria-hidden="true"></i>                                 </th>                                 <th class="connection-type-column">CONNECTION TYPE</th>                                 <th class="user-status-column">STATUS</th>                             </tr>                         </thead>                         <tbody>                             <tr *ngFor="let user of usersOnCurrentPage">                                 <td class="client-id-column" (click)="initializeListeningToUserPage(user)">{{user.ClientID}}</td>                                 <td *ngIf="user.CertificateARN" class="connection-type-column" (click)="initializeListeningToUserPage(user)">OPENSSL</td>                                 <td *ngIf="!user.CertificateARN" class="connection-type-column" (click)="initializeListeningToUserPage(user)">WEBSOCKET</td>                                 <td class="user-status-column" (click)="initializeListeningToUserPage(user)">{{user.RegistrationStatus}}</td>                                 <td>                                     <action-stub-items class="float-right" [model]="user" [custom]="user.RegistrationStatus == \'BANNED\' ? bannedUserActions : registeredUserActions"></action-stub-items>                                 </td>                             </tr>                         </tbody>                     </table>                 </div>                 <pagination #pagination [pages]="userPages"                             (pageChanged)="updatePaginationInfo($event)">                 </pagination>             </div>         </div>     </div>     <div *ngIf="isListeningToUser">         <div class="row asb-container">             <action-stub-basic class="col-6 client-asb" [model]="currentUser" [custom]="currentUser.RegistrationStatus == \'BANNED\' ? bannedUserActions : registeredUserProfileActions">                 <div>                     {{currentUser.ClientID}}                 </div>                 <div>                     {{currentUser.RegistrationStatus}}                 </div>             </action-stub-basic>         </div>         <h2 class="activity-header">             Current Activity for              <select [(ngModel)]="channelType" class="channel-type" (ngModelChange)="listenToUser(currentUser.ClientID, channelType)">                 <option [value]="\'allChannels\'">All channels</option>                 <option *ngFor="let channel of simplifiedChannels" [value]="channel.ChannelName">{{channel.ChannelName}}</option>             </select>         </h2>         <div class="activity">             <div class="activity-message" *ngFor="let message of messageList">{{message}}</div>         </div>     </div> </div> <div *ngIf="subNavActiveIndex == 1">     <div *ngIf="!isListeningToChannel">         <div [ngSwitch]="isLoadingChannels || isRegisteringClient">             <div *ngSwitchCase="true">                 <div class="loading-spinner-container">                     <loading-spinner></loading-spinner>                 </div>             </div>             <div class="content-container" *ngSwitchCase="false">                 <div *ngIf="!simplifiedChannels || simplifiedChannels.length == 0">                     No channels                 </div>                 <div *ngIf="simplifiedChannels.length > 0">                     <table class="table table-hover float-left">                         <thead>                             <tr>                                 <th class="channel-id-column" (click)="sort(simplifiedChannels, \'ChannelName\', !ascOrder)">                                     CHANNEL ID                                     <i *ngIf="ascOrder" class="fa fa-sort-amount-asc" aria-hidden="true"></i>                                     <i *ngIf="!ascOrder" class="fa fa-sort-amount-desc" aria-hidden="true"></i>                                 </th>                                 <th class="communication-type-column">COMMUNICATION TYPE</th>                             </tr>                         </thead>                         <tbody>                             <tr *ngFor="let channel of simplifiedChannels">                                 <td class="channel-id-column" (click)="initializeListeningToChannelPage(channel)">{{channel.ChannelName}}</td>                                 <td class="communication-type-column" (click)="initializeListeningToChannelPage(channel)">{{channel.CommunicationType}}</td>                                 <td>                                     <div class="float-right">                                         <action-stub-items class="float-right" [model]="channel" [custom]="channelActions"></action-stub-items>                                     </div>                                 </td>                             </tr>                         </tbody>                     </table>                 </div>             </div>         </div>     </div>     <div *ngIf="isListeningToChannel">         <button (click)="sendMessageViaChannelModal(currentChannel)" class="btn l-primary">             Send Message         </button>         <h2 class="activity-header">             Activity         </h2>         <div class="activity">             <div class="activity-message" *ngFor="let message of messageList">{{message}}</div>         </div>     </div> </div>  \x3c!-- SendMessageViaChannel modal --\x3e <modal *ngIf="mode == webCommunicatorModes.SendMessageViaChannel"        title="Send message"        [autoOpen]="true"        [metricIdentifier]="context.identifier"        [hasSubmit]="true"        [onClose]="dismissModal"        [onDismiss]="dismissModal"        (modalSubmitted)="sendMessageViaChannel()"        submitButtonText="Send Message">     <div class="modal-body">         <div class="form-group row">             <label class="col-3 col-form-label affix">Client ID</label>             <div class="col-9">                 <select *ngIf="getChannelCommunicationType(selectedChannel) != \'BROADCAST\'" class="form-control" [(ngModel)]="selectedUser" name="message-receiver">                     <option *ngIf="getChannelCommunicationType(selectedChannel) != \'PRIVATE\'" [value]="\'allClients\'">All Clients</option>                     <option *ngFor="let user of users" [value]="user.ClientID">{{user.ClientID}}</option>                 </select>                 <span *ngIf="getChannelCommunicationType(selectedChannel) == \'BROADCAST\'">All Clients</span>             </div>         </div>         <div class="form-group row">             <label class="col-3 col-form-label affix ">Channel</label>             <div class="col-9">                 <span>{{selectedChannel}}</span>             </div>         </div>         <div class="form-group row">             <label class="col-3 col-form-label affix">Message</label>             <div class="col-9">                 <textarea class="form-control" [(ngModel)]="newMessage" name="broadcast-message" cols="100" rows="5"></textarea>             </div>         </div>     </div> </modal>  \x3c!-- SendMessageToUser modal --\x3e <modal *ngIf="mode == webCommunicatorModes.SendMessageToUser"        title="Send message"        [autoOpen]="true"        [metricIdentifier]="context.identifier"        [hasSubmit]="true"        [onClose]="dismissModal"        [onDismiss]="dismissModal"        (modalSubmitted)="sendMessageToUser()"        submitButtonText="Send Message">     <div class="modal-body">         <div class="form-group row">             <label class="col-3 col-form-label affix">Client ID</label>             <div class="col-9">                 <span>{{currentUser.ClientID}}</span>             </div>         </div>         <div class="form-group row">             <label class="col-3 col-form-label affix ">Channel</label>             <div class="col-9">                 <select class="form-control" [(ngModel)]="selectedChannel" name="send-message" placeholder="Enter your message here">                     <ng-container *ngFor="let channel of simplifiedChannels">                         <option *ngIf="channel.CommunicationType != \'BROADCAST\'" [value]="channel.ChannelName">{{channel.ChannelName}}</option>                     </ng-container>                 </select>             </div>         </div>         <div class="form-group row">             <label class="col-3 col-form-label affix">Message</label>             <div class="col-9">                 <textarea class="form-control" [(ngModel)]="newMessage" name="broadcast-message" cols="100" rows="5"></textarea>             </div>         </div>     </div> </modal>  \x3c!-- BanUser modal --\x3e <modal *ngIf="mode == webCommunicatorModes.BanUser"        title="Ban User"        [autoOpen]="true"        [hasSubmit]="true"        [onDismiss]="dismissModal"        [onClose]="dismissModal"        [submitButtonText]="\'Ban\'"        (modalSubmitted)="banUser(currentUser)"        [metricIdentifier]="context.identifier">     <div class="modal-body">         <p> Are you sure you want to ban this user?</p>         <pre>{{currentUser | json | devonly}}</pre>     </div> </modal>',styles:[".content-container{margin-bottom:30px}.search-label{margin:5px 5px 5px 5px}table.table{margin-bottom:30px}table.table thead{display:table-header-group}table.table input:focus{outline:none}table.table tr .client-id-column{width:50%}table.table tr .connection-type-column{width:20%}table.table tr .user-status-column{width:20%}table.table tr .channel-id-column{width:60%}table.table tr .communication-type-column{width:30%}.user-profile{width:30%;border:1px solid #6441A5;padding:10px}.activity-header{margin-top:20px}.activity{width:100%;height:500px;overflow:auto;background-color:#f8f8f8}.activity-message{font-family:Courier}.channel-type,.user-type{width:200px}.client-asb{margin-bottom:32px;margin-top:10px}.asb-container .row info{margin:0 5px}"]}),q("design:paramtypes",[c.Http,f.AwsService,i.ToastsManager,d.ViewContainerRef,l.LyMetricService,l.BreadcrumbService])],b)}(h.AbstractCloudGemIndexComponent),a("WebCommunicatorIndexComponent",n)}}}),a.register("55",["10","52","11","6"],function(a,b){"use strict";var c,d,e,f,g,h=this&&this.__decorate||function(a,b,c,d){var e,f=arguments.length,g=f<3?b:null===d?d=Object.getOwnPropertyDescriptor(b,c):d;if("object"==typeof Reflect&&"function"==typeof Reflect.decorate)g=Reflect.decorate(a,b,c,d);else for(var h=a.length-1;h>=0;h--)(e=a[h])&&(g=(f<3?e(g):f>3?e(b,c,g):e(b,c))||g);return f>3&&g&&Object.defineProperty(b,c,g),g};b&&b.id;return{setters:[function(a){c=a},function(a){d=a},function(a){e=a},function(a){f=a}],execute:function(){g=function(){function a(){}return a=h([f.NgModule({imports:[e.GameSharedModule,c.GemModule],declarations:[d.WebCommunicatorIndexComponent,d.WebCommunicatorThumbnailComponent],providers:[],bootstrap:[d.WebCommunicatorThumbnailComponent,d.WebCommunicatorIndexComponent]})],a)}(),a("CloudGemWebCommunicatorModule",g)}}}),a.register("56",[],function(a,b){"use strict";var c,d;b&&b.id;return{setters:[],execute:function(){c=function(){function a(a){this.ClientID=a.ClientID,this.RegistrationStatus=a.RegistrationStatus,this.RegistrationDate=a.RegistrationDate,this.CertificateARN=a.CertificateARN}return a}(),a("UserEntry",c),d=function(){function a(a){this.ChannelName=a.ChannelName,this.CommunicationType=a.CommunicationType,this.CommunicationChannel=a.CommunicationChannel,this.Subscription=a.Subscription,this.BroadcastChannelName=a.Subscription}return a}(),a("ChannelEntry",d)}}}),a.register("52",["50","51","53","55","56"],function(a,b){"use strict";function c(b){var c={};for(var d in b)"default"!==d&&(c[d]=b[d]);a(c)}b&&b.id;return{setters:[function(a){c(a)},function(a){c(a)},function(a){c(a)},function(a){c(a)},function(a){c(a)}],execute:function(){}}}),a.register("4f",["52"],function(a,b){"use strict";function c(){return d.CloudGemWebCommunicatorModule}b&&b.id;a("definition",c);var d;return{setters:[function(a){d=a}],execute:function(){}}})})(function(a){if("function"==typeof define&&define.amd)define(["app/shared/class/index.js","app/view/game/module/cloudgems/class/index.js","@angular/core","@angular/http","app/aws/aws.service.js","app/shared/service/index.js","app/shared/component/index.js","ng2-toastr/ng2-toastr.js","app/view/game/module/shared/class/index.js","ng2-mqtt/mqttws31.js","app/view/game/module/cloudgems/gem.module.js","app/view/game/module/shared/shared.module.js"],a);else{if("object"!=typeof module||!module.exports||"function"!=typeof require)throw new Error("Module must be loaded as AMD or CommonJS");module.exports=a(require("app/shared/class/index.js"),require("app/view/game/module/cloudgems/class/index.js"),require("@angular/core"),require("@angular/http"),require("app/aws/aws.service.js"),require("app/shared/service/index.js"),require("app/shared/component/index.js"),require("ng2-toastr/ng2-toastr.js"),require("app/view/game/module/shared/class/index.js"),require("ng2-mqtt/mqttws31.js"),require("app/view/game/module/cloudgems/gem.module.js"),require("app/view/game/module/shared/shared.module.js"))}});