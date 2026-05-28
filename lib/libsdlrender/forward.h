#ifndef FORWARD_H
#define FORWARD_H

#define ForwardDefinition(type, name) typedef type name name##_t

ForwardDefinition(struct, camera);
ForwardDefinition(struct, appstate);
ForwardDefinition(struct, banner);
ForwardDefinition(struct, text_object);
ForwardDefinition(struct, map);

#endif
